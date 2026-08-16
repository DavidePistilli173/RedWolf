#pragma once

#ifdef RW_ENABLE_VULKAN

    #include "redwolf/common.hpp"
    #include "redwolf/containers/vec.hpp"

    #include <string_view>
    #include <vulkan/vulkan.h>

namespace rw {
    /**
     * @brief Renderer backend that deals with the Vulkan API.
     */
    class RendererBackend {
     public:
        ~RendererBackend();

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
        /**
         * @brief Initialise the Vulkan layer names.
         * @return Vector containing all required layer names.
         */
        [[nodiscard]] static Vec<const char*> init_layer_names_();

        /**
         * @brief Initialise the Vulkan debugger.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_vk_debugger_();

        /**
         * @brief Initialise the Vulkan instance.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_vk_instance_();

        RendererBackend() = default;

        VkInstance                 vk_instance_{}; /**< Vulkan instance. */
        Ptr<VkAllocationCallbacks> vk_allocator_;  /**< Custom Vulkan allocator. */

    #ifdef RW_ENABLE_VULKAN_DEBUG
        VkDebugUtilsMessengerEXT vk_debug_messenger_{}; /**< Vulkan debug messenger. */
    #endif
    };
} // namespace rw

#endif