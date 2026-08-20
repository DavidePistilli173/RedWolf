#pragma once

#include "redwolf/common.hpp"

#include <optional>
#include <vulkan/vulkan.h>

namespace rw {
    /**
     * @brief Vulkan rendering device.
     */
    class VulkanDeviceImpl {
     public:
        /**
         * @brief Detect the depth buffer format for the device and store it.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool detect_depth_format();

        /**
         * @brief Find the index of the memory area with the appropriate parameters.
         * @param type_filter Memory type filter.
         * @param property_flags Additional memory properties.
         * @return Index of the memory area, if available.
         */
        [[nodiscard]] static std::optional<u32> find_memory_index(u32 type_filter, u32 property_flags);

        /**
         * @brief Initialise the device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Get the swapchain support information from a physical device and store it.
         * @param device Device to check.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool query_swapchain_support(VkPhysicalDevice device);

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
         * @brief Select a suitable physical device.
         */
        [[nodiscard]] static bool select_physical_device_();
    };
} // namespace rw