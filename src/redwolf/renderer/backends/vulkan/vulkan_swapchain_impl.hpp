#pragma once

#include "redwolf/common.hpp"

#include <chrono>
#include <optional>
#include <vulkan/vulkan.h>

namespace rw {
    /**
     * @brief Vulkan image swapchain.
     */
    class VulkanSwapchainImpl {
     public:
        /**
         * @brief Initialise the swapchain.
         * @param width Viewport width.
         * @param height Viewport height.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init(u32 width, u32 height);

        /**
         * @brief Acquire the index of the next swapchain image.
         * @param timeout Timeout for waiting for the next image to become available.
         * @param image_available_semaphore Semaphore for waiting on the next image availability.
         * @param fence Fence
         * @return Image index, if available.
         */
        [[nodiscard]] static std::optional<u32>
            next_image_index(std::chrono::nanoseconds timeout, VkSemaphore image_available_semaphore, VkFence fence);

        /**
         * @brief Present an image from the swapchain.
         * @param render_complete_semaphore Semaphore that signals when rendering on the given image is complete.
         * @param present_image_index Index of the image to present.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool present(VkSemaphore render_complete_semaphore, u32 present_image_index);

        /**
         * @brief Recreate the swapchain.
         * @param width New viewport width.
         * @param height New viewport height.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool recreate(u32 width, u32 height);

        /**
         * @brief Shut down the swapchain.
         */
        static void shutdown();

     private:
        /**
         * @brief Create the swapchain.
         * @param width Viewport width.
         * @param height Viewport height.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool create_(u32 width, u32 height);

        /**
         * @brief Destroy the swapchain.
         */
        static void destroy_();
    };
} // namespace rw