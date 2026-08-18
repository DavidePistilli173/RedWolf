#pragma once

#include "redwolf/containers/vec.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/memory/ptr.hpp"

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define RW_VK_CHECK(res, message, ret_val)        \
    if (VK_SUCCESS != res) {                      \
        rw::error(message, string_VkResult(res)); \
        return ret_val;                           \
    }

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

    /*
     * @brief Vulkan device data.
     */
    struct VulkanDevice {
        VkPhysicalDevice physical{ VK_NULL_HANDLE };       /**< Hardware device that will do the rendering. */
        VkDevice         logical{ VK_NULL_HANDLE };        /**< Logical rendering device. */
        VkQueue          graphics_queue{ VK_NULL_HANDLE }; /**< Graphics queue. */
        VkQueue          present_queue{ VK_NULL_HANDLE };  /**< Presentation queue. */
        VkQueue          transfer_queue{ VK_NULL_HANDLE }; /**< Transfer queue. */

        VulkanPhysicalDeviceInfo    requirements;      /**< Physical device requirements. */
        VulkanDeviceQueueFamilyInfo queue_families;    /**< Queue family information. */
        VulkanSwapchainSupportInfo  swapchain_support; /**< Swapchain support information. */
    };

    /**
     * @brief All raw Vulkan data.
     */
    struct VulkanContext {
        VkInstance                 instance{ VK_NULL_HANDLE }; /**< Vulkan instance. */
        Ptr<VkAllocationCallbacks> allocator;                  /**< Custom Vulkan allocator. */
        VkSurfaceKHR               surface{ VK_NULL_HANDLE };  /**< Drawing surface. */
        VulkanDevice               device;                     /**< Rendering device. */

#ifdef RW_ENABLE_VULKAN_DEBUG
        VkDebugUtilsMessengerEXT debug_messenger{}; /**< Vulkan debug messenger. */
#endif

        /**
         * @brief Retrieve the global Vulkan context.
         */
        [[nodiscard]] static VulkanContext& ctx();

        /**
         * @brief Initialise the context.
         */
        static void init();

        /**
         * @brief Shutdown the context.
         */
        static void shutdown();
    };

} // namespace rw