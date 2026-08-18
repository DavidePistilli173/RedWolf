#pragma once

#include <vulkan/vulkan.h>

namespace rw {
    /**
     * @brief Vulkan rendering device.
     */
    class VulkanDeviceImpl {
     public:
        /**
         * @brief Initialise the device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Shutdown the device.
         */
        static void shutdown();

     private:
        /**
         * @brief Check if a device meets all requirements for the engine.
         * @param device Device to check.
         * @param properties Device properties.
         * @param features Device features.
         * @return true if the device meets the requirements, false otherwise.
         */
        [[nodiscard]] static bool are_physical_requirements_met_(
            VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features);

        /**
         * @brief Create the logical device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool create_logical_device_();

        /**
         * @brief Get all device queues.
         */
        static void get_device_queues_();

        /**
         * @brief Get the swapchain support information from a physical device and store it.
         * @param device Device to check.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool query_swapchain_support_(VkPhysicalDevice device);

        /**
         * @brief Select a suitable physical device.
         */
        [[nodiscard]] static bool select_physical_device_();
    };
} // namespace rw