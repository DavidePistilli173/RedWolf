#include "vulkan_renderpass.hpp"

#include "redwolf/renderer/backend/vulkan_command_buffer.hpp"
#include "vulkan_common.hpp"

#include <vulkan/vulkan_core.h>

rw::vk::RenderPass::~RenderPass() {
    if (VK_NULL_HANDLE != render_pass_) {
        vkDestroyRenderPass(device_->logical(), render_pass_, allocator_);
    }
}

void rw::vk::RenderPass::begin(Ptr<CommandBuffer> command_buffer, VkFramebuffer frame_buffer) {
    command_buffer_ = std::move(command_buffer);

    const std::array clear_values{ VkClearValue{
                                       .color = { .float32 = { clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a } } },
                                   VkClearValue{ .depthStencil = { .depth = depth_, .stencil = stencil_ } } };

    const VkRenderPassBeginInfo begin_info{
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext           = nullptr,
        .renderPass      = render_pass_,
        .framebuffer     = frame_buffer,
        .renderArea      = { .offset = { .x = static_cast<i32>(render_area_.x), .y = static_cast<i32>(render_area_.y) },
                             .extent = { .width = static_cast<u32>(render_area_.w), .height = static_cast<u32>(render_area_.h) } },
        .clearValueCount = clear_values.size(),
        .pClearValues    = clear_values.data()
    };

    vkCmdBeginRenderPass(command_buffer_->handle(), &begin_info, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer->set_state(CommandBuffer::State::in_render_pass);
}

void rw::vk::RenderPass::end() {
    vkCmdEndRenderPass(command_buffer_->handle());
    command_buffer_->set_state(CommandBuffer::State::recording);
    command_buffer_.reset(); // Release the command buffer from this render pass.
}

VkRenderPass rw::vk::RenderPass::handle() const {
    return render_pass_;
}

bool rw::vk::RenderPass::init(const Params& params) {
    allocator_ = params.allocator;
    device_    = params.device;
    swapchain_ = params.swapchain;

    // Attachments.
    // Color attachment.
    const VkAttachmentDescription color_attachment{ .flags          = 0,
                                                    .format         = swapchain_->image_format().format,
                                                    .samples        = VK_SAMPLE_COUNT_1_BIT,
                                                    .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                    .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                                                    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                    .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                                                    .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR };
    const VkAttachmentReference   color_attachment_reference{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    // Depth attachment.
    const VkAttachmentDescription depth_attachment{ .flags          = 0,
                                                    .format         = device_->depth_format(),
                                                    .samples        = VK_SAMPLE_COUNT_1_BIT,
                                                    .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                    .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                    .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                                                    .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
    const VkAttachmentReference   depth_attachment_reference{ .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    const std::array attachment_descriptions{ color_attachment, depth_attachment };

    // Main subpass.
    const VkSubpassDescription main_subpass{ .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
                                             .inputAttachmentCount    = 0,
                                             .pInputAttachments       = nullptr,
                                             .colorAttachmentCount    = 1,
                                             .pColorAttachments       = &color_attachment_reference,
                                             .pResolveAttachments     = nullptr,
                                             .pDepthStencilAttachment = &depth_attachment_reference,
                                             .preserveAttachmentCount = 0,
                                             .pPreserveAttachments    = nullptr };

    // Render pass dependecies.
    const VkSubpassDependency dependency{ .srcSubpass      = VK_SUBPASS_EXTERNAL,
                                          .dstSubpass      = 0,
                                          .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                          .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                          .srcAccessMask   = 0,
                                          .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                          .dependencyFlags = 0 };

    // Render pass creation.
    const VkRenderPassCreateInfo render_pass_create_info{ .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                                                          .pNext           = nullptr,
                                                          .flags           = 0,
                                                          .attachmentCount = attachment_descriptions.size(),
                                                          .pAttachments    = attachment_descriptions.data(),
                                                          .subpassCount    = 1,
                                                          .pSubpasses      = &main_subpass,
                                                          .dependencyCount = 1,
                                                          .pDependencies   = &dependency };

    RW_VK_CHECK(
        vkCreateRenderPass(device_->logical(), &render_pass_create_info, allocator_, &render_pass_),
        "Failed to create render pass: '{}'",
        false)

    return true;
}

void rw::vk::RenderPass::set_area_size(f32 width, f32 height) {
    render_area_.w = width;
    render_area_.h = height;
}
