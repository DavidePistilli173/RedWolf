#include "vulkan_renderer.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/user_data.hpp"
#include "redwolf/version_info.hpp"

#include <array>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_wayland.h>

namespace {
    /**
     * @brief Extensions required for Vulkan rendering.
     */
    constexpr std::array required_extensions{ VK_KHR_SURFACE_EXTENSION_NAME
#ifdef RW_ENABLE_VULKAN_DEBUG
                                              ,
                                              VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
    };

    VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             types,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void*                                       user_data) {
        switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            rw::error("{}", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            rw::warn("{}", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            rw::info("{}", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            rw::trace("{}", callback_data->pMessage);
            break;
        default:
            rw::error("{}", callback_data->pMessage);
            break;
        }
        return VK_FALSE;
    }

    rw::RendererBackend* g_backend{ nullptr }; // Renderer backend instance.
} // namespace

rw::RendererBackend::~RendererBackend() {
    vkDestroySurfaceKHR(vk_instance_, vk_surface_, vk_allocator_.get());

#ifdef RW_ENABLE_VULKAN_DEBUG
    auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(vk_instance_, "vkDestroyDebugUtilsMessengerEXT")) };
    if (nullptr == func) {
        error("Failed to get function 'vkDestroyDebugUtilsMessengerEXT'.");
    } else {
        func(vk_instance_, vk_debug_messenger_, vk_allocator_.get());
    }
#endif

    vkDestroyInstance(vk_instance_, vk_allocator_.get());
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

    if (!g_backend->init_vk_debugger_()) {
        error("Failed to initialise Vulkan debugger.");
        return false;
    }

    if (!g_backend->init_vk_surface_()) {
        error("Failed to initialise Vulkan surface.");
        return false;
    }

    if (!g_backend->device_.init(g_backend->vk_instance_, g_backend->vk_surface_)) {
        error("Failed to initialise rendering device.");
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

rw::Vec<const char*> rw::RendererBackend::init_layer_names_() {
    Vec<const char*> result{ MemoryType::renderer };
#ifdef RW_ENABLE_VULKAN_DEBUG

    trace("Validation layers enabled.");
    (void) result.emplace_back("VK_LAYER_KHRONOS_validation");

    // Check that the layers exist.
    u32 available_layer_count{ 0U };
    if (const auto res{ vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr) }; VK_SUCCESS != res) {
        error("Failed to enumerate Vulkan layers: '{}'", string_VkResult(res));
        return Vec<const char*>(MemoryType::renderer);
    }

    Vec<VkLayerProperties> layer_properties{ MemoryType::renderer };
    layer_properties.resize(available_layer_count);
    if (const auto res{ vkEnumerateInstanceLayerProperties(&available_layer_count, layer_properties.data()) }; VK_SUCCESS != res) {
        error("Failed to enumerate Vulkan layers: '{}'", string_VkResult(res));
        return Vec<const char*>(MemoryType::renderer);
    }

    for (const auto& required_layer : result) {
        bool found{ false };
        for (const auto& available_layer : layer_properties) {
            if (0 == std::strcmp(required_layer, available_layer.layerName)) {
                found = true;
                break;
            }
        }

        if (!found) {
            error("Layer '{}' not available.", required_layer);
            return Vec<const char*>(MemoryType::renderer);
        }
    }

#endif
    return result;
}

bool rw::RendererBackend::init_vk_debugger_() {
#ifdef RW_ENABLE_VULKAN_DEBUG
    const VkDebugUtilsMessengerCreateInfoEXT debug_create_info{ .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                                                                .flags           = 0,
                                                                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                                                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
                                                                .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                                                                .pfnUserCallback = &vk_debug_callback,
                                                                .pUserData       = nullptr };

    auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(vk_instance_, "vkCreateDebugUtilsMessengerEXT")) };
    if (nullptr == func) {
        error("Failed to get function 'vkCreateDebugUtilsMessengerEXT'.");
        return false;
    }

    if (const auto res{ func(vk_instance_, &debug_create_info, vk_allocator_.get(), &vk_debug_messenger_) }; VK_SUCCESS != res) {
        error("Failed to create Vulkan debug messenger.");
        return false;
    }

    trace("Vulkan debugger created.");
#endif

    return true;
}

bool rw::RendererBackend::init_vk_instance_() {
    const VkApplicationInfo app_info{ .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                      .pApplicationName = UserData::app_name().c_str(),

                                      .applicationVersion = VK_MAKE_VERSION(
                                          UserData::app_version().major, UserData::app_version().minor, UserData::app_version().patch),
                                      .pEngineName   = "RedWolf",
                                      .engineVersion = VK_MAKE_VERSION(rw::version.major, rw::version.minor, rw::version.patch),
                                      .apiVersion    = VK_API_VERSION_1_2 };

    Vec<const char*> enabled_extensions{ MemoryType::renderer, required_extensions };
    Platform::append_required_extension_names(enabled_extensions);

    Vec<const char*> enabled_layers{ init_layer_names_() };

    const VkInstanceCreateInfo create_info{ .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                            .pApplicationInfo        = &app_info,
                                            .enabledLayerCount       = static_cast<u32>(enabled_layers.size()),
                                            .ppEnabledLayerNames     = enabled_layers.data(),
                                            .enabledExtensionCount   = static_cast<u32>(enabled_extensions.size()),
                                            .ppEnabledExtensionNames = enabled_extensions.data() };

    if (const auto res{ vkCreateInstance(&create_info, vk_allocator_.get(), &vk_instance_) }; VK_SUCCESS != res) {
        error("Failed to create Vulkan instance: '{}'", string_VkResult(res));
        return false;
    }

    return true;
}

bool rw::RendererBackend::init_vk_surface_() {
    const VkWaylandSurfaceCreateInfoKHR create_info{ .sType   = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
                                                     .flags   = 0,
                                                     .display = Platform::display(),
                                                     .surface = Platform::surface() };

    if (const auto res{ vkCreateWaylandSurfaceKHR(vk_instance_, &create_info, vk_allocator_.get(), &vk_surface_) }; VK_SUCCESS != res) {
        error("Failed to create Vulkan surface: '{}'", string_VkResult(res));
        return false;
    }

    return true;
}
