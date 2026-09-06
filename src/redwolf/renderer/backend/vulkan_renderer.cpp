#include "vulkan_renderer.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/memory/memory.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/renderer/backend/vulkan_command_buffer.hpp"
#include "redwolf/renderer/backend/vulkan_renderpass.hpp"
#include "redwolf/user_data.hpp"
#include "redwolf/version_info.hpp"
#include "vulkan_common.hpp"
#include "vulkan_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_swapchain.hpp"

#include <array>
#include <ranges>

namespace {
    constexpr std::chrono::nanoseconds default_timeout{ std::numeric_limits<u64>::max() };

    rw::RendererBackend* g_backend{ nullptr }; // Renderer backend instance.
} // namespace

rw::RendererBackend::~RendererBackend() {
    // Wait for the device to be idle before proceeding with destruction.
    if (const auto res{ vkDeviceWaitIdle(device_->logical()) }; !vk::result_not_error(res)) {
        error("vkDeviceWaitIdle failed: '{}'", string_VkResult(res));
    }
}

bool rw::RendererBackend::begin_frame(f32 delta_time) {
    return g_backend->begin_frame_(delta_time);
}

bool rw::RendererBackend::end_frame(f32 delta_time) {
    return g_backend->end_frame_(delta_time);
}

bool rw::RendererBackend::init() {
    if (nullptr != g_backend) {
        warn("Renderer backend already initialised.");
        return true;
    }
    g_backend = new RendererBackend();

    if (!g_backend->init_internal_()) {
        error("Failed to initialise the renderer backend.");
        return false;
    }

    return true;
}

void rw::RendererBackend::on_resize(const WindowResizeEvent& event) {
    g_backend->cached_framebuffer_width_  = event.new_width;
    g_backend->cached_framebuffer_height_ = event.new_height;
    ++g_backend->framebuffer_size_generation_;
    info("Vulkan renderer backend resized: '{}x{}'", event.new_width, event.new_height);
}

void rw::RendererBackend::shutdown() {
    if (nullptr == g_backend) {
        warn("Renderer backend already shut down.");
        return;
    }

    delete g_backend;
    g_backend = nullptr;
}

bool rw::RendererBackend::begin_frame_(f32 delta_time) {
    if (recreating_swapchain_) {
        if (const auto res{ vkDeviceWaitIdle(device_->logical()) }; !vk::result_not_error(res)) {
            error("vkDeviceWaitIdle failed: '{}'", string_VkResult(res));
            return false;
        }
        info("Recreating swapchain, skipping frame.");
        return false;
    }

    // Check if the swapchain needs to be recreated.
    if ((framebuffer_size_generation_ != framebuffer_size_last_generation_) || swapchain_->needs_recreation()) {
        if (const auto res{ vkDeviceWaitIdle(device_->logical()) }; !vk::result_not_error(res)) {
            error("vkDeviceWaitIdle failed: '{}'", string_VkResult(res));
            return false;
        }

        if (!recreate_swapchain_()) {
            return false;
        }

        // This frame has to be skipped.
        info("Framebuffer resized. Skipping frame.");
        return false;
    }

    current_frame_ = swapchain_->current_frame();
    if (!in_flight_fences_[current_frame_]->wait(default_timeout)) {
        warn("Failed to wait on the in-flight fence.");
        return false;
    }

    // Acquire the next image index.
    const auto image_index_res{ swapchain_->next_image_index(
        default_timeout, image_available_sems_[current_frame_]->handle(), VK_NULL_HANDLE) };
    if (!image_index_res.has_value()) {
        warn("Failed to retrieve index of the next image.");
        return false;
    }
    image_index_ = image_index_res.value();

    // Begin recording commands.
    const auto& command_buffer{ graphics_command_buffers_[image_index_] };
    command_buffer->reset();
    if (!command_buffer->begin(false, false, false)) {
        error("Failed to begin command buffer recording.");
        return false;
    }

    const VkViewport viewport{ .x        = 0.0F,
                               .y        = static_cast<f32>(framebuffer_height_),
                               .width    = static_cast<f32>(framebuffer_width_),
                               .height   = -static_cast<f32>(framebuffer_height_), // OpenGL convention.
                               .minDepth = 0.0F,
                               .maxDepth = 1.0F };

    const VkRect2D scissor{ .offset = { .x = 0, .y = 0 }, .extent = { .width = framebuffer_width_, .height = framebuffer_height_ } };

    vkCmdSetViewport(command_buffer->handle(), 0, 1, &viewport);
    vkCmdSetScissor(command_buffer->handle(), 0, 1, &scissor);

    main_renderpass_->set_area_size(static_cast<f32>(framebuffer_width_), static_cast<f32>(framebuffer_height_));
    main_renderpass_->begin(command_buffer, framebuffers_[image_index_]->handle());

    return true;
}

bool rw::RendererBackend::create_command_buffers_() {
    graphics_command_buffers_.resize(swapchain_->image_count());

    for (auto& buffer : graphics_command_buffers_) {
        if (!buffer.is_null()) {
            buffer->reset();
        }

        buffer = std::move(Memory::new_object<vk::CommandBuffer>(MemoryCategory::renderer));
        if (!buffer->init(device_, device_->graphics_command_pool(), true)) {
            error("Failed to initialise command buffer.");
            return false;
        }
    }

    return true;
}

bool rw::RendererBackend::end_frame_(f32 delta_time) {
    main_renderpass_->end();
    if (!graphics_command_buffers_[image_index_]->end()) {
        error("Failed to end command buffer recording.");
        return false;
    }

    // Make sure that the previous frame is not this image.
    if (nullptr != images_in_flight_[image_index_]) {
        if (!images_in_flight_[image_index_]->wait(default_timeout)) {
            warn("Failed to wait on current image fence.");
            return false;
        }
    }

    // Mark the fence as in use.
    images_in_flight_[image_index_] = in_flight_fences_[current_frame_].get();

    // Reset the fence for use on the next frame.
    if (!in_flight_fences_[current_frame_]->reset()) {
        error("Failed to reset fence.");
        return false;
    }

    // Queue submission.
    const std::array<VkPipelineStageFlags, 1> pipeline_flags{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    const VkSubmitInfo submit_info{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                    .pNext = nullptr,
                                    // Wait semaphore ensures that the operation cannot begin until the image is available.
                                    .waitSemaphoreCount = 1,
                                    .pWaitSemaphores    = image_available_sems_[current_frame_]->handle_pointer(),
                                    // Each semaphore waits on the corresponding pipeline stage to complete.
                                    .pWaitDstStageMask  = pipeline_flags.data(),
                                    .commandBufferCount = 1,
                                    .pCommandBuffers    = graphics_command_buffers_[image_index_]->handle_pointer(),
                                    // Semaphore to be signalled when the queue is complete.
                                    .signalSemaphoreCount = 1,
                                    .pSignalSemaphores    = queue_complete_sems_[image_index_]->handle_pointer() };

    RW_VK_CHECK(
        vkQueueSubmit(device_->graphics_queue(), 1, &submit_info, in_flight_fences_[current_frame_]->handle()),
        "Failed to submit queue: '{}'",
        false)

    graphics_command_buffers_[image_index_]->update_submitted();

    // Presentation.
    if (!swapchain_->present(queue_complete_sems_[image_index_]->handle(), image_index_)) {
        error("Failed to present frame.");
        return false;
    }

    return true;
}

bool rw::RendererBackend::regenerate_framebuffers_() {
    framebuffers_.resize(swapchain_->image_count());

    for (auto [i, framebuffer] : std::views::enumerate(framebuffers_)) {
        framebuffer = Memory::new_object<vk::Framebuffer>(MemoryCategory::renderer);

        Vec<VkImageView> attachments{ MemoryCategory::renderer };
        (void) attachments.emplace_back(swapchain_->views()[i]);
        (void) attachments.emplace_back(swapchain_->depth_buffer()->view());

        if (!framebuffer->init(
                vk::Framebuffer::Params{ .allocator   = allocator_,
                                         .device      = device_,
                                         .attachments = attachments,
                                         .render_pass = main_renderpass_,
                                         .width       = framebuffer_width_,
                                         .height      = framebuffer_height_ })) {
            error("Failed to initialise framebuffer.");
            return false;
        }
    }

    return true;
}

bool rw::RendererBackend::init_internal_() {
    instance_ = Memory::new_object<vk::Instance>(MemoryCategory::renderer);
    if (!instance_->init(allocator_)) {
        error("Failed to initialise Vulkan instance.");
        return false;
    }

    surface_ = Memory::new_object<vk::Surface>(MemoryCategory::renderer);
    if (!surface_->init(g_backend->instance_, allocator_)) {
        error("Failed to initialise Vulkan surface.");
        return false;
    }

    device_ = Memory::new_object<vk::Device>(MemoryCategory::renderer);
    if (!device_->init(instance_, allocator_, surface_)) {
        error("Failed to initialise rendering device.");
        return false;
    }

    swapchain_ = Memory::new_object<vk::Swapchain>(MemoryCategory::renderer);
    if (!swapchain_->init(
            vk::Swapchain::Params{ .allocator = allocator_,
                                   .surface   = surface_,
                                   .device    = device_,
                                   .width     = framebuffer_width_,
                                   .height    = framebuffer_height_ })) {
        error("Failed to initialise swapchain.");
        return false;
    }

    main_renderpass_ = Memory::new_object<vk::RenderPass>(MemoryCategory::renderer);
    if (!main_renderpass_->init(
            vk::RenderPass::Params{
                .allocator   = allocator_,
                .device      = device_,
                .swapchain   = swapchain_,
                .render_area = { .x = 0, .y = 0, .w = static_cast<f32>(framebuffer_width_), .h = static_cast<f32>(framebuffer_height_) },
                .clear_color = { .r = 0.2F, .g = 0.0F, .b = 0.0F, .a = 1.0F },
                .depth       = 1.0F,
                .stencil     = 0 })) {
        error("Failed to initialise main render pass.");
        return false;
    }

    if (!regenerate_framebuffers_()) {
        error("Failed to regenerate framebuffers.");
        return false;
    }

    if (!create_command_buffers_()) {
        error("Failed to create command buffers.");
        return false;
    }

    if (!init_sync_objects_()) {
        error("Failed to create synchronisation objects.");
        return false;
    }

    window_resize_event_ = Events::subscribe<WindowResizeEvent>([this](const WindowResizeEvent& event) {
        on_resize(event);
        return false;
    });

    return true;
}

bool rw::RendererBackend::init_sync_objects_() {
    image_available_sems_.resize(swapchain_->max_frames_in_flight());
    for (auto& sem : image_available_sems_) {
        sem = Memory::new_object<vk::Semaphore>(MemoryCategory::renderer);
        if (!sem->init(allocator_, device_)) {
            error("Failed to initialise image available semaphore.");
            return false;
        }
    }

    queue_complete_sems_.resize(swapchain_->image_count());
    for (auto& sem : queue_complete_sems_) {
        sem = Memory::new_object<vk::Semaphore>(MemoryCategory::renderer);
        if (!sem->init(allocator_, device_)) {
            error("Failed to initialise queue complete semaphore.");
            return false;
        }
    }

    in_flight_fences_.resize(swapchain_->max_frames_in_flight());
    for (auto& fence : in_flight_fences_) {
        fence = Memory::new_object<vk::Fence>(MemoryCategory::renderer);
        if (!fence->init(allocator_, device_, true)) {
            error("Failed to initialise in-flight fences.");
            return false;
        }
    }

    images_in_flight_.resize(swapchain_->image_count());

    return true;
}

bool rw::RendererBackend::recreate_swapchain_() {
    if (recreating_swapchain_) {
        warn("Already recreating swapchain.");
        return false;
    }

    if ((0 == framebuffer_width_) || (0 == framebuffer_height_)) {
        warn("Recreate swapchain called with size {}x{}", framebuffer_width_, framebuffer_height_);
        return false;
    }

    recreating_swapchain_ = true;

    if (const auto res{ vkDeviceWaitIdle(device_->logical()) }; !vk::result_not_error(res)) [[unlikely]] {
        error("Failed to wait on device: '{}'", string_VkResult(res));
        return false;
    }

    if (!device_->query_swapchain_support(device_->physical())) [[unlikely]] {
        error("Failed to query device swapchain support.");
        return false;
    }

    if (!device_->detect_depth_format()) [[unlikely]] {
        error("Failed to detect depth format.");
        return false;
    }

    if (!swapchain_->recreate(cached_framebuffer_width_, cached_framebuffer_height_)) {
        error("Failed to recreate swapchain.");
        return false;
    }

    // Clear the images in flight.
    images_in_flight_.resize(swapchain_->image_count());
    images_in_flight_.fill(nullptr);

    if (queue_complete_sems_.size() != swapchain_->image_count()) {
        queue_complete_sems_.clear();
        queue_complete_sems_.resize(swapchain_->image_count());
        for (auto& sem : queue_complete_sems_) {
            sem = Memory::new_object<vk::Semaphore>(MemoryCategory::renderer);
            if (!sem->init(allocator_, device_)) {
                error("Failed to re-initialise queue complete semaphore.");
                return false;
            }
        }
    }

    framebuffer_width_         = cached_framebuffer_width_;
    framebuffer_height_        = cached_framebuffer_height_;
    cached_framebuffer_width_  = 0;
    cached_framebuffer_height_ = 0;

    framebuffer_size_last_generation_ = framebuffer_size_generation_;

    // Clear command buffers and framebuffers.
    graphics_command_buffers_.clear();
    framebuffers_.clear();

    main_renderpass_->set_area_size(static_cast<f32>(framebuffer_width_), static_cast<f32>(framebuffer_height_));

    if (!regenerate_framebuffers_()) {
        error("Failed to regenerate framebuffers.");
        return false;
    }

    if (!create_command_buffers_()) {
        error("Failed to recreate command buffers");
        return false;
    }

    recreating_swapchain_ = false;

    return true;
}