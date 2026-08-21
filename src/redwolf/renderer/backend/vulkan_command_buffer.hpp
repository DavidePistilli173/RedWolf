#pragma once

#include "redwolf/common.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace rw::vk {
    /**
     * @brief Command buffer wrapper.
     */
    class CommandBuffer {
     public:
        /**
         * @brief States of the command buffer.
         */
        enum class State : u8 { not_allocated, ready, recording, in_render_pass, recording_ended, submitted };

        /**
         * @brief Get the raw command buffer handle.
         */
        [[nodiscard]] VkCommandBuffer handle() const;

        /**
         * @brief Set the state of the command buffer.
         * @param state New state to set.
         */
        void set_state(State state);

     private:
        VkCommandBuffer command_buffer_{ VK_NULL_HANDLE }; /**< Raw command buffer handle. */
        State           state_{ State::not_allocated };    /**< Current command buffer state. */
    };
} // namespace rw::vk