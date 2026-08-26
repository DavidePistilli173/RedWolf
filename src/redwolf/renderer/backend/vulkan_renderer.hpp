#pragma once

#include "redwolf/common.hpp"
#include "redwolf/containers/vec.hpp"
#include "redwolf/platform/platform_common.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_renderpass.hpp"
#include "vulkan_semaphore.hpp"
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
         * @param event Event data.
         */
        static void on_resize(const WindowResizeEvent& event);

        /**
         * @brief Shutdown the renderer backend.
         */
        static void shutdown();

     private:
        RendererBackend() = default;

        /**
         * @brief Start a frame to render.
         * @param delta_time Time since the last frame. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool begin_frame_(f32 delta_time);

        /**
         * @brief Create the main command buffers needed by the renderer.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool create_command_buffers_();

        /**
         * @brief End the current rendering frame.
         * @param delta_time Time since the last frame. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool end_frame_(f32 delta_time);

        /**
         * @brief Regenerate all the framebuffers required for rendering.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool regenerate_framebuffers_();

        /**
         * @brief Initialise the backend.
         */
        [[nodiscard]] bool init_internal_();

        /**
         * @brief Initialise the synchronisation objects.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_sync_objects_();

        VkAllocationCallbacks*      allocator_{ nullptr };                 /**< Custom vulkan allocator. */
        Ptr<vk::Instance>           instance_;                             /**< Backend instance. */
        Ptr<vk::Surface>            surface_;                              /**< Rendering surface. */
        Ptr<vk::Device>             device_;                               /**< Rendering device. */
        Ptr<vk::Swapchain>          swapchain_;                            /**< Swapchain for presenting images to the screen. */
        Ptr<vk::RenderPass>         main_renderpass_;                      /**< Main render pass. */
        Vec<Ptr<vk::Framebuffer>>   framebuffers_{ MemoryType::renderer }; /**< Framebuffers for the swapchain. */
        Vec<Ptr<vk::CommandBuffer>> graphics_command_buffers_{ MemoryType::renderer }; /**< Graphics command buffers. */

        Vec<Ptr<vk::Semaphore>> image_available_sems_{ MemoryType::renderer }; /**< Image available semaphores. */
        Vec<Ptr<vk::Semaphore>> queue_complete_sems_{ MemoryType::renderer };  /**< Queue completion semaphores. */
        Vec<Ptr<vk::Fence>>     in_flight_fences_{ MemoryType::renderer };     /**< Fences for the in-flight frames. */
        Vec<VkFence>            images_in_flight_{ MemoryType::renderer };     /**< Fences currently in use. */

        u32 cached_framebuffer_width_{ 512U };       /**< Cached framebuffer width, to be used when resizing. */
        u32 cached_framebuffer_height_{ 512U };      /**< Cached framebuffer height, to be used when resizing. */
        u64 framebuffer_size_generation_{ 0U };      /**< Counter for detecting framebuffer size changes. */
        u64 framebuffer_size_last_generation_{ 0U }; /**< Last time the framebuffer was resized. */
        u32 framebuffer_width_{ 512U };              /**< Framebuffer width. */
        u32 framebuffer_height_{ 512U };             /**< Framebuffer height. */

        usize current_frame_{ 0U };           /**< Index of the current frame. */
        usize image_index_{ 0U };             /**< Index of the current image. */
        bool  recreating_swapchain_{ false }; /**< True if we are recreating the swapchain. */
    };
} // namespace rw
