#pragma once

#include "redwolf/common.hpp"
#include "redwolf/containers/vec.hpp"
#include "vulkan_device.hpp"
#include "vulkan_image.hpp"
#include "vulkan_surface.hpp"

#include <chrono>
#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace rw::vk {
    /**
     * @brief Vulkan image swapchain.
     */
    class Swapchain {
     public:
        /**
         * @brief Initialisation parameters.
         */
        struct Params {
            VkAllocationCallbacks* allocator{ nullptr }; /**< Custom vulkan allocator. */
            const Ptr<Surface>&    surface;              /**< Rendering surface. */
            const Ptr<Device>&     device;               /**< Rendering device. */
            u32                    width{ 0U };          /**< Viewport width. */
            u32                    height{ 0U };         /**< Viewport height. */
        };

        Swapchain() = default;
        ~Swapchain();

        Swapchain(const Swapchain&)            = delete;
        Swapchain& operator=(const Swapchain&) = delete;

        Swapchain(Swapchain&&)            = delete;
        Swapchain& operator=(Swapchain&&) = delete;

        /**
         * @brief Get the index of the currently drawn frame.
         */
        [[nodiscard]] usize current_frame() const;

        /**
         * @brief Get the depth attachment.
         */
        [[nodiscard]] const Ptr<Image2D>& depth_buffer() const;

        /**
         * @brief Get the number of images in the swapchain.
         */
        [[nodiscard]] usize image_count() const;

        /**
         * @brief Get the swapchain image format.
         */
        [[nodiscard]] VkSurfaceFormatKHR image_format() const;

        /**
         * @brief Initialise the swapchain.
         * @param params Initialisation parameters.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(const Params& params);

        /**
         * @brief Get the maximum number of in-flight frames.
         */
        [[nodiscard]] u8 max_frames_in_flight() const;

        /**
         * @brief Check whether the swapchain needs to be re-created.
         */
        [[nodiscard]] bool needs_recreation() const;

        /**
         * @brief Acquire the index of the next swapchain image.
         * @param timeout Timeout for waiting for the next image to become available.
         * @param image_available_semaphore Semaphore for waiting on the next image availability.
         * @param fence Fence
         * @return Image index, if available.
         */
        [[nodiscard]] std::optional<u32>
            next_image_index(std::chrono::nanoseconds timeout, VkSemaphore image_available_semaphore, VkFence fence);

        /**
         * @brief Present an image from the swapchain.
         * @param render_complete_semaphore Semaphore that signals when rendering on the given image is complete.
         * @param present_image_index Index of the image to present.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool present(VkSemaphore render_complete_semaphore, u32 present_image_index);

        /**
         * @brief Recreate the swapchain.
         * @param width New viewport width.
         * @param height New viewport height.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool recreate(u32 width, u32 height);

        /**
         * @brief Get all the image views.
         */
        [[nodiscard]] const Vec<VkImageView>& views() const;

     private:
        /**
         * @brief Create the swapchain.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool create_();

        /**
         * @brief Destroy the swapchain.
         */
        void destroy_();

        VkAllocationCallbacks* allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<Surface>           surface_;              /**< Rendering surface. */
        Ptr<Device>            device_;               /**< Rendering device. */

        VkSwapchainKHR     swapchain_{ VK_NULL_HANDLE };    /**< Raw swapchain handle. */
        VkSurfaceFormatKHR image_format_{};                 /**< Image data format. */
        u8                 max_frames_in_flight_{ 2 };      /**< Maximum number of frames being rendered to. */
        Vec<VkImage>       images_{ MemoryType::renderer }; /**< Swapchain images. */
        Vec<VkImageView>   views_{ MemoryType::renderer };  /**< Image views. */
        Ptr<Image2D>       depth_buffer_;                   /**< Depth buffer. */

        u32 width_{ 0U };  /**< Viewport width. */
        u32 height_{ 0U }; /**< Viewport height. */

        usize current_frame_{ 0U };       /**< Index of the frame that is currently being rendered. */
        bool  needs_recreation_{ false }; /**< If true, the swapchain needs to be recreated. */
    };
} // namespace rw::vk