#pragma once

#include "redwolf/containers/vec.hpp"
#include "redwolf/memory/memory_pool.hpp"

#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace rw {
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

    /**
     * @brief Vulkan rendering device.
     */
    class VulkanDevice {
     public:
        VulkanDevice() = default;

        /**
         * @brief Initialise the device.
         * @param vk_instance Vulkan instance.
         * @param vk_surface Vulkan surface.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(VkInstance vk_instance, VkSurfaceKHR vk_surface);

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
         * @brief Select a suitable physical device.
         */
        [[nodiscard]] bool select_physical_device_();

        VkInstance       vk_instance_{}; /**< Vulkan instance. */
        VkSurfaceKHR     vk_surface_{};  /**< Vulkan surface. */
        VkPhysicalDevice vk_physical_{}; /**< Physical device. */
        VkDevice         vk_logical_{};  /**< Logical device. */

        VulkanPhysicalDeviceInfo    requirements_;      /**< Physical device requirements. */
        VulkanDeviceQueueFamilyInfo queue_families_;    /**< Queue family information. */
        VulkanSwapchainSupportInfo  swapchain_support_; /**< Swapchain support information. */
    };

    /**
     * @brief Initialise a new Vulkan device.
     * @param vk_instance Vulkan instance.
     * @return Newly created device, if no errors occured.
     */
    [[nodiscard]] std::optional<VulkanDevice> vulkan_device_init(VkInstance vk_instance);
} // namespace rw