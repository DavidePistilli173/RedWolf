#pragma once

#include "redwolf/common.hpp"
#include "vulkan_device.hpp"

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

        CommandBuffer() = default;
        ~CommandBuffer();

        CommandBuffer(const CommandBuffer&)            = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;

        CommandBuffer(CommandBuffer&&)            = delete;
        CommandBuffer& operator=(CommandBuffer&&) = delete;

        /**
         * @brief Begin recording the command buffer.
         */
        void begin(bool is_single_use, bool is_renderpass_continue, bool is_simultaneous_use);

        /**
         * @brief End the current command buffer recording.
         */
        void end();

        /**
         * @brief End the recording of a single use command buffer and submit it.
         */
        void end_and_submit_single_use();

        /**
         * @brief Initialise the command buffer.
         * @param device Rendering device.
         * @param command_pool Command pool the buffer will be allocated from.
         * @param is_primary true if this is a primary command buffer, false if it is a secondary command buffer.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(Ptr<Device> device, VkCommandPool command_pool, bool is_primary);

        /**
         * @brief Initialise and begin recording a single use command buffer.
         * @param allocator Custom vulkan allocator.
         * @param device Rendering device.
         * @param command_pool Command pool the buffer will be allocated from.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_and_begin_single_use(Ptr<Device> device, VkCommandPool command_pool);

        /**
         * @brief Get the raw command buffer handle.
         */
        [[nodiscard]] VkCommandBuffer handle() const;

        /**
         * @brief Reset the command buffer to the ready state.
         */
        void reset();

        /**
         * @brief Set the state of the command buffer.
         * @param state New state to set.
         */
        void set_state(State state);

        /**
         * @brief Update the command buffer to the submitted state.
         */
        void update_submitted();

     private:
        Ptr<Device>   device_;                         /**< Rendering device. */
        VkCommandPool command_pool_{ VK_NULL_HANDLE }; /**< Raw handle to the command pool. */

        VkCommandBuffer command_buffer_{ VK_NULL_HANDLE }; /**< Raw command buffer handle. */
        State           state_{ State::not_allocated };    /**< Current command buffer state. */
    };
} // namespace rw::vk