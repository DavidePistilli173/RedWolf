#include "vulkan_renderer.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/renderer/backend/vulkan_device.hpp"
#include "redwolf/renderer/backend/vulkan_instance.hpp"
#include "redwolf/renderer/backend/vulkan_surface.hpp"
#include "redwolf/renderer/backend/vulkan_swapchain.hpp"
#include "redwolf/user_data.hpp"
#include "redwolf/version_info.hpp"
#include "vulkan_common.hpp"

#include <array>

namespace {
    rw::RendererBackend* g_backend{ nullptr }; // Renderer backend instance.
} // namespace

bool rw::RendererBackend::begin_frame(f32 delta_time) {
    return true;
}

bool rw::RendererBackend::end_frame(f32 delta_time) {
    return true;
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

void rw::RendererBackend::shutdown() {
    if (nullptr == g_backend) {
        warn("Renderer backend already shut down.");
        return;
    }

    delete g_backend;
    g_backend = nullptr;
}

bool rw::RendererBackend::init_internal_() {
    instance_ = Memory::new_object<vk::Instance>(MemoryType::renderer);
    if (!instance_->init(allocator_)) {
        error("Failed to initialise Vulkan instance.");
        return false;
    }

    surface_ = Memory::new_object<vk::Surface>(MemoryType::renderer);
    if (!surface_->init(g_backend->instance_, allocator_)) {
        error("Failed to initialise Vulkan surface.");
        return false;
    }

    device_ = Memory::new_object<vk::Device>(MemoryType::renderer);
    if (!device_->init(instance_, allocator_, surface_)) {
        error("Failed to initialise rendering device.");
        return false;
    }

    swapchain_ = Memory::new_object<vk::Swapchain>(MemoryType::renderer);
    if (!swapchain_->init(
            vk::Swapchain::Params{ .allocator = allocator_,
                                   .surface   = surface_,
                                   .device    = device_,
                                   .width     = framebuffer_width_,
                                   .height    = framebuffer_height_ })) {
        error("Failed to initialise swapchain.");
        return false;
    }

    return true;
}
