#pragma once

#include "redwolf/color.hpp"
#include "redwolf/common.hpp"
#include "redwolf/memory/ptr.hpp"
#include "redwolf/rect.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_swapchain.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace rw::vk {
    /**
     * @brief Render pass wrapper.
     */
    class RenderPass {
     public:
        /**
         * @brief Possible states of the render pass.
         */
        enum class State : u8 { not_allocated, ready, recording, in_render_pass, recording_ended, submitted };

        /**
         * @brief Initialisation parameters.
         */
        struct Params {
            VkAllocationCallbacks* allocator{ nullptr }; /**< Custom vulkan allocator. */
            Ptr<Device>            device;               /**< Rendering device. */
            Ptr<Swapchain>         swapchain;            /**< Swapchain. */
            Rect                   render_area;          /**< Area to render to. */
            Color                  clear_color;          /**< Colour used to clear the screen. */
            f32                    depth{ 0.0F };
            u32                    stencil{ 0U };
        };

        RenderPass() = default;
        ~RenderPass();

        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;

        RenderPass(RenderPass&&)            = delete;
        RenderPass& operator=(RenderPass&&) = delete;

        /**
         * @brief Begin the render pass.
         * @param command_buffer Command buffer used to record the render pass.
         * @param frame_buffer Frame buffer used as target for the render pass operations.
         */
        void begin(Ptr<CommandBuffer> command_buffer, VkFramebuffer frame_buffer);

        /**
         * @brief End the render pass.
         */
        void end();

        /**
         * @brief Initialise the render pass.
         * @param params Initialisation parameters.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(const Params& params);

     private:
        VkAllocationCallbacks* allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<Device>            device_;               /**< Rendering device. */
        Ptr<Swapchain>         swapchain_;            /**< Swapchain. */
        Ptr<CommandBuffer>     command_buffer_;       /**< Command buffer used for the render pass if active. */

        VkRenderPass render_pass_{ VK_NULL_HANDLE }; /**< Raw render pass handle. */
        State        state_{ State::not_allocated }; /**< Current state of the render pass. */
        Rect         render_area_;                   /**< Area to render to. */
        Color        clear_color_;                   /**< Colour used to clear the screen. */
        f32          depth_{ 0.0F };
        u32          stencil_{ 0U };
    };
} // namespace rw::vk