#include "vulkan_instance.hpp"

#include "redwolf/platform/platform.hpp"
#include "redwolf/user_data.hpp"
#include "redwolf/version_info.hpp"
#include "vulkan_common.hpp"

namespace {
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
} // namespace

rw::vk::Instance::~Instance() {
#ifdef RW_ENABLE_VULKAN_DEBUG
    auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT")) };
    if (nullptr == func) {
        error("Failed to get function 'vkDestroyDebugUtilsMessengerEXT'.");
    } else {
        func(instance_, debug_messenger_, allocator_);
    }
#endif

    vkDestroyInstance(instance_, allocator_);
}

VkInstance rw::vk::Instance::handle() const {
    return instance_;
}

bool rw::vk::Instance::init(VkAllocationCallbacks* allocator) {
    allocator_ = allocator;

    if (!init_instance_()) {
        error("Failed to initialise instance.");
        return false;
    }

    if (!init_debugger_()) {
        error("Failed to initialise debugger.");
        return false;
    }

    return true;
}

bool rw::vk::Instance::init_debugger_() {
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

    auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT")) };
    if (nullptr == func) {
        error("Failed to get function 'vkCreateDebugUtilsMessengerEXT'.");
        return false;
    }

    if (const auto res{ func(instance_, &debug_create_info, allocator_, &debug_messenger_) }; VK_SUCCESS != res) {
        error("Failed to create Vulkan debug messenger.");
        return false;
    }

    trace("Vulkan debugger created.");
#endif

    return true;
}

bool rw::vk::Instance::init_instance_() {
    const VkApplicationInfo app_info{ .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                      .pApplicationName = UserData::app_name().c_str(),

                                      .applicationVersion = VK_MAKE_VERSION(
                                          UserData::app_version().major, UserData::app_version().minor, UserData::app_version().patch),
                                      .pEngineName   = "RedWolf",
                                      .engineVersion = VK_MAKE_VERSION(rw::version.major, rw::version.minor, rw::version.patch),
                                      .apiVersion    = VK_API_VERSION_1_2 };

    Vec<const char*> enabled_extensions{ MemoryCategory::renderer, required_extensions };
    Platform::append_required_extension_names(enabled_extensions);

    Vec<const char*> enabled_layers{ init_layer_names_() };

    const VkInstanceCreateInfo create_info{ .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                            .pApplicationInfo        = &app_info,
                                            .enabledLayerCount       = static_cast<u32>(enabled_layers.size()),
                                            .ppEnabledLayerNames     = enabled_layers.data(),
                                            .enabledExtensionCount   = static_cast<u32>(enabled_extensions.size()),
                                            .ppEnabledExtensionNames = enabled_extensions.data() };

    RW_VK_CHECK(vkCreateInstance(&create_info, allocator_, &instance_), "Failed to create Vulkan instance: '{}'", false)
    return true;
}

rw::Vec<const char*> rw::vk::Instance::init_layer_names_() {
    Vec<const char*> result{ MemoryCategory::renderer };
#ifdef RW_ENABLE_VULKAN_DEBUG

    trace("Validation layers enabled.");
    (void) result.emplace_back("VK_LAYER_KHRONOS_validation");

    // Check that the layers exist.
    u32 available_layer_count{ 0U };
    RW_VK_CHECK(
        vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr),
        "Failed to enumerate Vulkan layers: '{}'",
        Vec<const char*>(MemoryCategory::renderer))

    Vec<VkLayerProperties> layer_properties{ MemoryCategory::renderer };
    layer_properties.resize(available_layer_count);
    RW_VK_CHECK(
        vkEnumerateInstanceLayerProperties(&available_layer_count, layer_properties.data()),
        "Failed to enumerate Vulkan layers: '{}'",
        Vec<const char*>(MemoryCategory::renderer))

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
            return Vec<const char*>(MemoryCategory::renderer);
        }
    }

#endif
    return result;
}