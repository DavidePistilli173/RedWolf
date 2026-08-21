#pragma once

#include "redwolf/common.hpp"
#include "redwolf/containers/vec.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"

#include <limits>
#include <optional>
#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan rendering device.
     */
    class Device {
     public:
        /**
         * @brief Information about a physical device.
         */
        struct VulkanPhysicalDeviceInfo {
            bool             graphics{ false };                            /**< Has a graphics queue. */
            bool             present{ false };                             /**< Has a presentation queue. */
            bool             compute{ false };                             /**< Has a compute queue. */
            bool             transfer{ false };                            /**< Has a transfer queue. */
            Vec<const char*> supported_extensions{ MemoryType::renderer }; /**< Supported extensions. */
            bool             sampler_anisotropy{ false };                  /**< */
            bool             discrete{ false };                            /**< Discrete/integrated GPU. */
        };

        /**
         * @brief Information about queue families of a device.
         */
        struct VulkanDeviceQueueFamilyInfo {
            u32 graphics_family_index{ std::numeric_limits<u32>::max() };
            u32 present_family_index{ std::numeric_limits<u32>::max() };
            u32 compute_family_index{ std::numeric_limits<u32>::max() };
            u32 transfer_family_index{ std::numeric_limits<u32>::max() };
        };

        /**
         * @brief Information about swapchain support for a device.
         */
        struct VulkanSwapchainSupportInfo {
            VkSurfaceCapabilitiesKHR capabilities{};                        /**< Capabilities. */
            Vec<VkSurfaceFormatKHR>  formats{ MemoryType::renderer };       /**< Image formats. */
            Vec<VkPresentModeKHR>    present_modes{ MemoryType::renderer }; /**< Presentation modes. */
        };

        Device() = default;
        ~Device();

        Device(const Device&)            = delete;
        Device& operator=(const Device&) = delete;

        Device(Device&&)            = delete;
        Device& operator=(Device&&) = delete;

        /**
         * @brief Get the supported depth format.
         */
        [[nodiscard]] VkFormat depth_format() const;

        /**
         * @brief Detect the depth buffer format for the device and store it.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool detect_depth_format();

        /**
         * @brief Find the index of the memory area with the appropriate parameters.
         * @param type_filter Memory type filter.
         * @param property_flags Additional memory properties.
         * @return Index of the memory area, if available.
         */
        [[nodiscard]] std::optional<u32> find_memory_index(u32 type_filter, u32 property_flags);

        /**
         * @brief Get the raw graphics queue handle.
         */
        [[nodiscard]] VkQueue graphics_queue() const;

        /**
         * @brief Initialise the device.
         * @param instance Vulkan instance.
         * @param allocator Custom vulkan allocator.
         * @param surface Rendering surface.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(Ptr<Instance> instance, VkAllocationCallbacks* allocator, Ptr<Surface> surface);

        /**
         * @brief Get the raw logical device handle.
         */
        [[nodiscard]] VkDevice logical() const;

        /**
         * @brief Get the raw physical device handle.
         */
        [[nodiscard]] VkPhysicalDevice physical() const;

        /**
         * @brief Get the raw presentation queue handle.
         */
        [[nodiscard]] VkQueue present_queue() const;

        /**
         * @brief Get the swapchain support information from a physical device and store it.
         * @param device Device to check.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool query_swapchain_support(VkPhysicalDevice device);

        /**
         * @brief Get the queue family information for the device.
         */
        [[nodiscard]] const VulkanDeviceQueueFamilyInfo& queue_families() const;

        /**
         * @brief Get the swapchain support information for the device.
         */
        [[nodiscard]] const VulkanSwapchainSupportInfo& swapchain_support() const;

     private:
        /**
         * @brief Check if a device meets all requirements for the engine.
         * @param device Device to check.
         * @param properties Device properties.
         * @param features Device features.
         * @return true if the device meets the requirements, false otherwise.
         */
        [[nodiscard]] bool are_physical_requirements_met_(
            VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features);

        /**
         * @brief Create the logical device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool create_logical_device_();

        /**
         * @brief Get all device queues.
         */
        void get_device_queues_();

        /**
         * @brief Select a suitable physical device.
         */
        [[nodiscard]] bool select_physical_device_();

        Ptr<Instance>          instance_;             /**< Vulkan instance. */
        VkAllocationCallbacks* allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<Surface>           surface_;              /**< Rendering surface. */

        VkPhysicalDevice physical_{ VK_NULL_HANDLE };       /**< Hardware device that will do the rendering. */
        VkDevice         logical_{ VK_NULL_HANDLE };        /**< Logical rendering device. */
        VkQueue          graphics_queue_{ VK_NULL_HANDLE }; /**< Graphics queue. */
        VkQueue          present_queue_{ VK_NULL_HANDLE };  /**< Presentation queue. */
        VkQueue          transfer_queue_{ VK_NULL_HANDLE }; /**< Transfer queue. */

        VulkanPhysicalDeviceInfo    requirements_;      /**< Physical device requirements. */
        VulkanDeviceQueueFamilyInfo queue_families_;    /**< Queue family information. */
        VulkanSwapchainSupportInfo  swapchain_support_; /**< Swapchain support information. */

        VkFormat depth_format_{}; /**< Depth buffer format. */
    };
} // namespace rw::vk