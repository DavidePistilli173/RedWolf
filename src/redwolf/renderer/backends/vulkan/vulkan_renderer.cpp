#include "vulkan_renderer.hpp"

#include <vulkan/vulkan_core.h>

#ifdef RW_ENABLE_VULKAN

    #include "redwolf/logger.hpp"
    #include "redwolf/user_data.hpp"
    #include "redwolf/version_info.hpp"

namespace {
    rw::RendererBackend* g_backend{ nullptr }; // Renderer backend instance.
}

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
    if (!g_backend->init_vk_instance_()) {
        error("Failed to initialise Vulkan instance.");
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

bool rw::RendererBackend::init_vk_instance_() {
    const VkApplicationInfo app_info{ .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                      .pApplicationName = UserData::app_name().c_str(),

                                      .applicationVersion = VK_MAKE_VERSION(
                                          UserData::app_version().major, UserData::app_version().minor, UserData::app_version().patch),
                                      .pEngineName   = "RedWolf",
                                      .engineVersion = VK_MAKE_VERSION(rw::version.major, rw::version.minor, rw::version.patch),
                                      .apiVersion    = VK_API_VERSION_1_2 };

    const VkInstanceCreateInfo create_info{ .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                            .pApplicationInfo        = &app_info,
                                            .enabledLayerCount       = 0,
                                            .ppEnabledLayerNames     = nullptr,
                                            .enabledExtensionCount   = 0,
                                            .ppEnabledExtensionNames = nullptr };

    if (const auto res{ vkCreateInstance(&create_info, nullptr, &vk_instance_) }; VK_SUCCESS != res) {
        error("Failed to create Vulkan instance: '{}'", static_cast<u32>(res));
        return false;
    }

    return true;
}

#endif