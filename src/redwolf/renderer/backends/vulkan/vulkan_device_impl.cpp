#include "vulkan_device_impl.hpp"

#include "redwolf/common.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "vulkan_common.hpp"

#include <limits>
#include <ranges>
#include <vulkan/vulkan_core.h>

namespace {
    /**
     * @brief Candidate formats for the depth buffer.
     */
    constexpr std::array<VkFormat, 3> depth_format_candidates {
        VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT
    }
} // namespace

bool rw::VulkanDeviceImpl::detect_depth_format() {
    auto& vk_ctx{ VulkanContext::ctx() };

    const u32 flags{ VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT };

    for (auto candidate : depth_format_candidates) {
        VkFormatProperties properties{};
        vkGetPhysicalDeviceFormatProperties(vk_ctx.device.physical, candidate, &properties);

        if ((flags == (properties.linearTilingFeatures & flags)) || (flags == (properties.optimalTilingFeatures & flags))) {
            vk_ctx.device.depth_format = candidate;
            return true;
        }
    }

    error("No suitable depth buffer format.");
    return false;
}

bool rw::VulkanDeviceImpl::init() {
    auto& vk_ctx{ VulkanContext::ctx() };
    vk_ctx.device.requirements.graphics           = true;
    vk_ctx.device.requirements.present            = true;
    vk_ctx.device.requirements.compute            = false;
    vk_ctx.device.requirements.transfer           = true;
    vk_ctx.device.requirements.sampler_anisotropy = true;
    vk_ctx.device.requirements.discrete           = true;
    (void) vk_ctx.device.requirements.supported_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    if (!select_physical_device_()) {
        error("Failed to select physical device.");
        return false;
    }

    if (!create_logical_device_()) {
        error("Failed to create logical device.");
        return false;
    }

    get_device_queues_();

    return true;
}

bool rw::VulkanDeviceImpl::query_swapchain_support(VkPhysicalDevice device) {
    auto& vk_ctx{ VulkanContext::ctx() };

    // Capabilities.
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_ctx.surface, &vk_ctx.device.swapchain_support.capabilities),
        "Failed to query device capabilities: '{}'",
        false)

    // Surface formats.
    u32 format_count{ 0U };
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_ctx.surface, &format_count, nullptr), "Failed to enumerate surface formats.", false)

    if (0 != format_count) {
        vk_ctx.device.swapchain_support.formats.resize(format_count);
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_ctx.surface, &format_count, nullptr),
            "Failed to retrieve surface formats.",
            false)
    }
    // Present modes.
    u32 present_modes{ 0U };
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_ctx.surface, &present_modes, nullptr),
        "Failed to enumerate surface present modes: '{}'",
        false)

    if (0 != present_modes) {
        vk_ctx.device.swapchain_support.present_modes.resize(present_modes);
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_ctx.surface, &present_modes, nullptr),
            "Failed to retrieve present modes: '{}'",
            false)
    }

    return true;
}

void rw::VulkanDeviceImpl::shutdown() {
    auto& vk_ctx{ VulkanContext::ctx() };
    if (VK_NULL_HANDLE != vk_ctx.device.logical) {
        vkDestroyDevice(vk_ctx.device.logical, vk_ctx.allocator.get());
    }

    vk_ctx.device = VulkanDevice{};
}

bool rw::VulkanDeviceImpl::are_physical_requirements_met_(
    VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features) {
    auto& vk_ctx{ VulkanContext::ctx() };

    // Check discrete requirement.
    if (vk_ctx.device.requirements.discrete) {
        if (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU != properties.deviceType) {
            return false;
        }
    }

    // Obtain queue family data.
    u32 queue_family_count{ 0U };
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    Vec<VkQueueFamilyProperties> queue_family_properties{ MemoryType::renderer };
    queue_family_properties.resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_properties.data());

    u8 min_transfer_score = std::numeric_limits<u8>::max();
    for (const auto [i, queue_family] : std::views::enumerate(queue_family_properties)) {
        u8 current_transfer_score{ 0U };

        // Graphics queue?
        if (0 != (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            vk_ctx.device.queue_families.graphics_family_index = i;
            ++current_transfer_score;
        }

        // Compute queue?
        if (0 != (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            vk_ctx.device.queue_families.compute_family_index = i;
            ++current_transfer_score;
        }

        // Transfer queue?
        if (0 != (queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
            // Take the index if it is the current lowest.
            // This increases the likelyhood that it is a dedicated transfer queue.
            if (current_transfer_score <= min_transfer_score) {
                min_transfer_score                                 = current_transfer_score;
                vk_ctx.device.queue_families.transfer_family_index = i;
            }
        }

        // Presentation.
        VkBool32 supports_present{ VK_FALSE };
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vk_ctx.surface, &supports_present),
            "Failed to check surface presentation support: '{}'",
            false)
        if (0 != supports_present) {
            vk_ctx.device.queue_families.present_family_index = i;
        }
    }

    info("Found device '{}'", properties.deviceName);
    info("Graphics queue: '{}'", vk_ctx.device.queue_families.graphics_family_index);
    info("Present queue: '{}'", vk_ctx.device.queue_families.present_family_index);
    info("Transfer queue: '{}'", vk_ctx.device.queue_families.transfer_family_index);
    info("Compute queue: '{}'", vk_ctx.device.queue_families.compute_family_index);

    // Check against requirements.
    if (vk_ctx.device.requirements.graphics && (std::numeric_limits<u32>::max() == vk_ctx.device.queue_families.graphics_family_index)) {
        info("Device not suitable: no graphics queue.");
        return false;
    }
    if (vk_ctx.device.requirements.present && (std::numeric_limits<u32>::max() == vk_ctx.device.queue_families.present_family_index)) {
        info("Device not suitable: no present queue.");
        return false;
    }
    if (vk_ctx.device.requirements.compute && (std::numeric_limits<u32>::max() == vk_ctx.device.queue_families.compute_family_index)) {
        info("Device not suitable: no compute queue.");
        return false;
    }
    if (vk_ctx.device.requirements.transfer && (std::numeric_limits<u32>::max() == vk_ctx.device.queue_families.transfer_family_index)) {
        info("Device not suitable: no transfer queue.");
        return false;
    }

    if (!query_swapchain_support(device)) {
        error("Failed to query swapchain support.");
        return false;
    }

    if (vk_ctx.device.swapchain_support.formats.empty() || vk_ctx.device.swapchain_support.present_modes.empty()) {
        info("Device not suitable: no surface formats or present modes.");
        return false;
    }

    // Check extensions.
    if (!vk_ctx.device.requirements.supported_extensions.empty()) {
        u32 extension_count{ 0U };
        RW_VK_CHECK(
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr), "Failed to enumerate extensions: '{}'", false)
        if (0 == extension_count) {
            info("Device unsuitable: no extensions supported.");
            return false;
        }
        Vec<VkExtensionProperties> extension_properties{ MemoryType::renderer };
        extension_properties.resize(extension_count);
        RW_VK_CHECK(
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties.data()),
            "Failed to retrieve extension properties: '{}'",
            false)

        for (const auto& req_extension : vk_ctx.device.requirements.supported_extensions) {
            if (const auto res{ extension_properties.find_first([&req_extension](const VkExtensionProperties& props) {
                    return 0 == std::strcmp(props.extensionName, req_extension);
                }) };
                !res.has_value()) {
                info("Extension '{}' is not supported by this device.", req_extension);
                return false;
            }
        }
    }

    // Check individual features.
    if (vk_ctx.device.requirements.sampler_anisotropy && !features.samplerAnisotropy) {
        info("Device does not support sampler anisotropy.");
        return false;
    }

    return true;
}

bool rw::VulkanDeviceImpl::create_logical_device_() {
    auto& vk_ctx{ VulkanContext::ctx() };

    // Do not create additional queues for shared indices.
    const bool present_shares_graphics_queue{ vk_ctx.device.queue_families.present_family_index ==
                                              vk_ctx.device.queue_families.graphics_family_index };
    const bool transfer_shares_graphics_queue{ vk_ctx.device.queue_families.transfer_family_index ==
                                               vk_ctx.device.queue_families.graphics_family_index };

    const std::array<f32, 2> queue_priority{ 1.0F, 1.0F };

    // Graphics queues.
    Vec<VkDeviceQueueCreateInfo> queue_create_info{ MemoryType::renderer };
    (void) queue_create_info.emplace_back(
        VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                 .pNext            = nullptr,
                                 .flags            = 0,
                                 .queueFamilyIndex = vk_ctx.device.queue_families.graphics_family_index,
                                 .queueCount       = 1,
                                 .pQueuePriorities = queue_priority.data() });

    // Presentation queue.
    if (!present_shares_graphics_queue) {
        (void) queue_create_info.emplace_back(
            VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                     .pNext            = nullptr,
                                     .flags            = 0,
                                     .queueFamilyIndex = vk_ctx.device.queue_families.present_family_index,
                                     .queueCount       = 1,
                                     .pQueuePriorities = queue_priority.data() });
    }

    // Transfer queue.
    if (!transfer_shares_graphics_queue) {
        (void) queue_create_info.emplace_back(
            VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                     .pNext            = nullptr,
                                     .flags            = 0,
                                     .queueFamilyIndex = vk_ctx.device.queue_families.transfer_family_index,
                                     .queueCount       = 1,
                                     .pQueuePriorities = queue_priority.data() });
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = vk_ctx.device.requirements.sampler_anisotropy ? VK_TRUE : VK_FALSE;

    const VkDeviceCreateInfo device_create_info{ .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                                 .pNext                   = nullptr,
                                                 .queueCreateInfoCount    = static_cast<u32>(queue_create_info.size()),
                                                 .pQueueCreateInfos       = queue_create_info.data(),
                                                 .enabledLayerCount       = 0,
                                                 .ppEnabledLayerNames     = nullptr,
                                                 .enabledExtensionCount   = 1,
                                                 .ppEnabledExtensionNames = vk_ctx.device.requirements.supported_extensions.data(),
                                                 .pEnabledFeatures        = &device_features };

    RW_VK_CHECK(
        vkCreateDevice(vk_ctx.device.physical, &device_create_info, vk_ctx.allocator.get(), &vk_ctx.device.logical),
        "Failed to create logical device: '{}'",
        false)

    return true;
}

void rw::VulkanDeviceImpl::get_device_queues_() {
    auto& vk_ctx{ VulkanContext::ctx() };
    vkGetDeviceQueue(vk_ctx.device.logical, vk_ctx.device.queue_families.graphics_family_index, 0, &vk_ctx.device.graphics_queue);
    vkGetDeviceQueue(vk_ctx.device.logical, vk_ctx.device.queue_families.present_family_index, 0, &vk_ctx.device.present_queue);
    vkGetDeviceQueue(vk_ctx.device.logical, vk_ctx.device.queue_families.transfer_family_index, 0, &vk_ctx.device.transfer_queue);
}

bool rw::VulkanDeviceImpl::select_physical_device_() {
    auto& vk_ctx{ VulkanContext::ctx() };

    u32 physical_device_count{ 0U };
    RW_VK_CHECK(
        vkEnumeratePhysicalDevices(vk_ctx.instance, &physical_device_count, nullptr), "Failed to enumerate physical devices: '{}'", false)

    if (0 == physical_device_count) {
        error("No physical devices support Vulkan.");
        return false;
    }

    Vec<VkPhysicalDevice> physical_devices{ MemoryType::renderer };
    physical_devices.resize(physical_device_count);
    RW_VK_CHECK(
        vkEnumeratePhysicalDevices(vk_ctx.instance, &physical_device_count, physical_devices.data()),
        "Failed to retrieve list of physical devices.",
        false)

    for (const auto dev : physical_devices) {
        VkPhysicalDeviceProperties dev_properties{};
        vkGetPhysicalDeviceProperties(dev, &dev_properties);

        VkPhysicalDeviceFeatures dev_features{};
        vkGetPhysicalDeviceFeatures(dev, &dev_features);

        VkPhysicalDeviceMemoryProperties dev_memory{};
        vkGetPhysicalDeviceMemoryProperties(dev, &dev_memory);

        if (are_physical_requirements_met_(dev, dev_properties, dev_features)) {
            info("Selected device: '{}'", dev_properties.deviceName);
            switch (dev_properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                info("Device type: '{}'", "OTHER");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                info("Device type: '{}'", "INTEGRATED GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                info("Device type: '{}'", "DISCRETE GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                info("Device type: '{}'", "VIRTUAL GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                info("Device type: '{}'", "CPU");
                break;
            default:
                info("Device type: '{}'", "UNKNOWN");
                break;
            }
            info(
                "Driver version: '{}.{}.{}'",
                VK_VERSION_MAJOR(dev_properties.driverVersion),
                VK_VERSION_MINOR(dev_properties.driverVersion),
                VK_VERSION_PATCH(dev_properties.driverVersion));
            info(
                "API version: '{}.{}.{}'",
                VK_VERSION_MAJOR(dev_properties.apiVersion),
                VK_VERSION_MINOR(dev_properties.apiVersion),
                VK_VERSION_PATCH(dev_properties.apiVersion));

            for (u32 i{ 0U }; i < dev_memory.memoryHeapCount; ++i) {
                const auto mem_size_gib{ static_cast<f32>(dev_memory.memoryHeaps[i].size * bytes_to_gibs) };
                if (0 != (dev_memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)) {
                    info("Local GPU memory: '{}' GiB", mem_size_gib);
                } else {
                    info("Shared system memory: '{}' GiB", mem_size_gib);
                }
            }

            vk_ctx.device.physical = dev;
            return true;
        }
    }

    return false;
}