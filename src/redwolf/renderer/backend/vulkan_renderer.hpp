#pragma once

#include "redwolf/common.hpp"
#include "redwolf/containers/vec.hpp"
#include "vulkan_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_renderpass.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_swapchain.hpp"

#include <string_view>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace rw {
    /**
     * @brief Renderer backend that deals with the Vulkan API.
     */
    class RendererBackend {
     public:
        ~RendererBackend() = default;

        RendererBackend(const RendererBackend&)            = delete;
        RendererBackend& operator=(const RendererBackend&) = delete;

        RendererBackend(RendererBackend&&)            = delete;
        RendererBackend& operator=(RendererBackend&&) = delete;

        /**
         * @brief Start a frame to render.
         * @param delta_time Time since the last frame. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool begin_frame(f32 delta_time);

        /**
         * @brief End the current rendering frame.
         * @param delta_time Time since the last frame. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool end_frame(f32 delta_time);

        /**
         * @brief Initialise the renderer backend.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Called on viewport resize.
         * @param width New width.
         * @param height New height.
         */
        static void on_resize(u16 width, u16 height);

        /**
         * @brief Shutdown the renderer backend.
         */
        static void shutdown();

     private:
        RendererBackend() = default;

        /**
         * @brief Initialise the backend.
         */
        [[nodiscard]] bool init_internal_();

        VkAllocationCallbacks* allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<vk::Instance>      instance_;             /**< Backend instance. */
        Ptr<vk::Surface>       surface_;              /**< Rendering surface. */
        Ptr<vk::Device>        device_;               /**< Rendering device. */
        Ptr<vk::Swapchain>     swapchain_;            /**< Swapchain for presenting images to the screen. */
        Ptr<vk::RenderPass>    main_renderpass_;      /**< Main render pass. */

        u32 framebuffer_width_{ 512U };  /**< Framebuffer width. */
        u32 framebuffer_height_{ 512U }; /**< Framebuffer height. */
    };
} // namespace rw
