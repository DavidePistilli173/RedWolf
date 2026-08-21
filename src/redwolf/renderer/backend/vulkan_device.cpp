#include "vulkan_device.hpp"

#include "redwolf/common.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "vulkan_common.hpp"

#include <limits>
#include <ranges>

namespace {
    /**
     * @brief Candidate formats for the depth buffer.
     */
    constexpr std::array<VkFormat, 3> depth_format_candidates{ VK_FORMAT_D32_SFLOAT,
                                                               VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                               VK_FORMAT_D24_UNORM_S8_UINT };
} // namespace

rw::vk::Device::~Device() {
    vkDestroyDevice(logical_, allocator_);
}

VkFormat rw::vk::Device::depth_format() const {
    return depth_format_;
}

bool rw::vk::Device::detect_depth_format() {
    const u32 flags{ VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT };

    for (auto candidate : depth_format_candidates) {
        VkFormatProperties properties{};
        vkGetPhysicalDeviceFormatProperties(physical_, candidate, &properties);

        if ((flags == (properties.linearTilingFeatures & flags)) || (flags == (properties.optimalTilingFeatures & flags))) {
            depth_format_ = candidate;
            return true;
        }
    }

    error("No suitable depth buffer format.");
    return false;
}

std::optional<u32> rw::vk::Device::find_memory_index(u32 type_filter, u32 property_flags) {
    VkPhysicalDeviceMemoryProperties memory_properties{};
    vkGetPhysicalDeviceMemoryProperties(physical_, &memory_properties);

    for (u32 i{ 0U }; i < memory_properties.memoryTypeCount; ++i) {
        if ((0 != (type_filter & (1 << i))) && ((memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags)) {
            return i;
        }
    }

    return {};
}

VkQueue rw::vk::Device::graphics_queue() const {
    return graphics_queue_;
}

bool rw::vk::Device::init(Ptr<Instance> instance, VkAllocationCallbacks* allocator, Ptr<Surface> surface) {
    instance_  = std::move(instance);
    allocator_ = allocator;
    surface_   = std::move(surface);

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

    if (!create_logical_device_()) {
        error("Failed to create logical device.");
        return false;
    }

    get_device_queues_();

    return true;
}

VkDevice rw::vk::Device::logical() const {
    return logical_;
}

VkPhysicalDevice rw::vk::Device::physical() const {
    return physical_;
}

VkQueue rw::vk::Device::present_queue() const {
    return present_queue_;
}

bool rw::vk::Device::query_swapchain_support(VkPhysicalDevice device) {
    // Capabilities.
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_->handle(), &swapchain_support_.capabilities),
        "Failed to query device capabilities: '{}'",
        false)

    // Surface formats.
    u32 format_count{ 0U };
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_->handle(), &format_count, nullptr),
        "Failed to enumerate surface formats.",
        false)

    if (0 != format_count) {
        swapchain_support_.formats.resize(format_count);
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_->handle(), &format_count, swapchain_support_.formats.data()),
            "Failed to retrieve surface formats.",
            false)
    }
    // Present modes.
    u32 present_modes{ 0U };
    RW_VK_CHECK(
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_->handle(), &present_modes, nullptr),
        "Failed to enumerate surface present modes: '{}'",
        false)

    if (0 != present_modes) {
        swapchain_support_.present_modes.resize(present_modes);
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_->handle(), &present_modes, nullptr),
            "Failed to retrieve present modes: '{}'",
            false)
    }

    return true;
}

const rw::vk::Device::VulkanDeviceQueueFamilyInfo& rw::vk::Device::queue_families() const {
    return queue_families_;
}

const rw::vk::Device::VulkanSwapchainSupportInfo& rw::vk::Device::swapchain_support() const {
    return swapchain_support_;
}

bool rw::vk::Device::are_physical_requirements_met_(
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

        // Presentation.
        VkBool32 supports_present{ VK_FALSE };
        RW_VK_CHECK(
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_->handle(), &supports_present),
            "Failed to check surface presentation support: '{}'",
            false)
        if (0 != supports_present) {
            queue_families_.present_family_index = i;
        }
    }

    info("Found device '{}'", properties.deviceName);
    info("Graphics queue: '{}'", queue_families_.graphics_family_index);
    info("Present queue: '{}'", queue_families_.present_family_index);
    info("Transfer queue: '{}'", queue_families_.transfer_family_index);
    info("Compute queue: '{}'", queue_families_.compute_family_index);

    // Check against requirements.
    if (requirements_.graphics && (std::numeric_limits<u32>::max() == queue_families_.graphics_family_index)) {
        info("Device not suitable: no graphics queue.");
        return false;
    }
    if (requirements_.present && (std::numeric_limits<u32>::max() == queue_families_.present_family_index)) {
        info("Device not suitable: no present queue.");
        return false;
    }
    if (requirements_.compute && (std::numeric_limits<u32>::max() == queue_families_.compute_family_index)) {
        info("Device not suitable: no compute queue.");
        return false;
    }
    if (requirements_.transfer && (std::numeric_limits<u32>::max() == queue_families_.transfer_family_index)) {
        info("Device not suitable: no transfer queue.");
        return false;
    }

    if (!query_swapchain_support(device)) {
        error("Failed to query swapchain support.");
        return false;
    }

    if (swapchain_support_.formats.empty() || swapchain_support_.present_modes.empty()) {
        info("Device not suitable: no surface formats or present modes.");
        return false;
    }

    // Check extensions.
    if (!requirements_.supported_extensions.empty()) {
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

        for (const auto& req_extension : requirements_.supported_extensions) {
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
    if (requirements_.sampler_anisotropy && !features.samplerAnisotropy) {
        info("Device does not support sampler anisotropy.");
        return false;
    }

    return true;
}

bool rw::vk::Device::create_logical_device_() {
    // Do not create additional queues for shared indices.
    const bool present_shares_graphics_queue{ queue_families_.present_family_index == queue_families_.graphics_family_index };
    const bool transfer_shares_graphics_queue{ queue_families_.transfer_family_index == queue_families_.graphics_family_index };

    const std::array<f32, 2> queue_priority{ 1.0F, 1.0F };

    // Graphics queues.
    Vec<VkDeviceQueueCreateInfo> queue_create_info{ MemoryType::renderer };
    (void) queue_create_info.emplace_back(
        VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                 .pNext            = nullptr,
                                 .flags            = 0,
                                 .queueFamilyIndex = queue_families_.graphics_family_index,
                                 .queueCount       = 1,
                                 .pQueuePriorities = queue_priority.data() });

    // Presentation queue.
    if (!present_shares_graphics_queue) {
        (void) queue_create_info.emplace_back(
            VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                     .pNext            = nullptr,
                                     .flags            = 0,
                                     .queueFamilyIndex = queue_families_.present_family_index,
                                     .queueCount       = 1,
                                     .pQueuePriorities = queue_priority.data() });
    }

    // Transfer queue.
    if (!transfer_shares_graphics_queue) {
        (void) queue_create_info.emplace_back(
            VkDeviceQueueCreateInfo{ .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                     .pNext            = nullptr,
                                     .flags            = 0,
                                     .queueFamilyIndex = queue_families_.transfer_family_index,
                                     .queueCount       = 1,
                                     .pQueuePriorities = queue_priority.data() });
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = requirements_.sampler_anisotropy ? VK_TRUE : VK_FALSE;

    const VkDeviceCreateInfo device_create_info{ .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                                 .pNext                   = nullptr,
                                                 .queueCreateInfoCount    = static_cast<u32>(queue_create_info.size()),
                                                 .pQueueCreateInfos       = queue_create_info.data(),
                                                 .enabledLayerCount       = 0,
                                                 .ppEnabledLayerNames     = nullptr,
                                                 .enabledExtensionCount   = 1,
                                                 .ppEnabledExtensionNames = requirements_.supported_extensions.data(),
                                                 .pEnabledFeatures        = &device_features };

    RW_VK_CHECK(vkCreateDevice(physical_, &device_create_info, allocator_, &logical_), "Failed to create logical device: '{}'", false)

    return true;
}

void rw::vk::Device::get_device_queues_() {
    vkGetDeviceQueue(logical_, queue_families_.graphics_family_index, 0, &graphics_queue_);
    vkGetDeviceQueue(logical_, queue_families_.present_family_index, 0, &present_queue_);
    vkGetDeviceQueue(logical_, queue_families_.transfer_family_index, 0, &transfer_queue_);
}

bool rw::vk::Device::select_physical_device_() {
    u32 physical_device_count{ 0U };
    RW_VK_CHECK(
        vkEnumeratePhysicalDevices(instance_->handle(), &physical_device_count, nullptr),
        "Failed to enumerate physical devices: '{}'",
        false)

    if (0 == physical_device_count) {
        error("No physical devices support Vulkan.");
        return false;
    }

    Vec<VkPhysicalDevice> physical_devices{ MemoryType::renderer };
    physical_devices.resize(physical_device_count);
    RW_VK_CHECK(
        vkEnumeratePhysicalDevices(instance_->handle(), &physical_device_count, physical_devices.data()),
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

            physical_ = dev;
            return true;
        }
    }

    return false;
}