#include "vulkan_device.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/memory/memory_pool.hpp"

#include <limits>
#include <ranges>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

bool rw::VulkanDevice::init(VkInstance vk_instance, VkSurfaceKHR vk_surface) {
    vk_instance_ = vk_instance;
    vk_surface_  = vk_surface;

    requirements_.graphics           = true;
    requirements_.present            = true;
    requirements_.compute            = false;
    requirements_.transfer           = true;
    requirements_.sampler_anisotropy = true;
    requirements_.discrete           = true;
    (void) requirements_.supported_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    if (!select_physical_device_()) {
        error("Failed to select physical device.");
        return false;
    }

    return true;
}

bool rw::VulkanDevice::are_physical_requirements_met_(
    VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features) {
    // Check discrete requirement.
    if (requirements_.discrete) {
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
            queue_families_.graphics_family_index = i;
            ++current_transfer_score;
        }

        // Compute queue?
        if (0 != (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            queue_families_.compute_family_index = i;
            ++current_transfer_score;
        }

        // Transfer queue?
        if (0 != (queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
            // Take the index if it is the current lowest.
            // This increases the likelyhood that it is a dedicated transfer queue.
            if (current_transfer_score <= min_transfer_score) {
                min_transfer_score                    = current_transfer_score;
                queue_families_.transfer_family_index = i;
            }
        }

        
    }
}

bool rw::VulkanDevice::select_physical_device_() {
    u32 physical_device_count{ 0U };
    if (const auto res{ vkEnumeratePhysicalDevices(vk_instance_, &physical_device_count, nullptr) }; VK_SUCCESS != res) {
        error("Failed to enumerate physical devices: '{}'", string_VkResult(res));
        return false;
    }

    if (0 == physical_device_count) {
        error("No physical devices support Vulkan.");
        return false;
    }

    Vec<VkPhysicalDevice> physical_devices{ MemoryType::renderer };
    physical_devices.resize(physical_device_count);
    if (const auto res{ vkEnumeratePhysicalDevices(vk_instance_, &physical_device_count, physical_devices.data()) }; VK_SUCCESS != res) {
        error("Failed to retrieve list of physical devices.");
        return false;
    }

    for (const auto dev : physical_devices) {
        VkPhysicalDeviceProperties dev_properties{};
        vkGetPhysicalDeviceProperties(dev, &dev_properties);

        VkPhysicalDeviceFeatures dev_features{};
        vkGetPhysicalDeviceFeatures(dev, &dev_features);

        VkPhysicalDeviceMemoryProperties dev_memory{};
        vkGetPhysicalDeviceMemoryProperties(dev, &dev_memory);

        VulkanPhysicalDeviceInfo info{};
    }
}