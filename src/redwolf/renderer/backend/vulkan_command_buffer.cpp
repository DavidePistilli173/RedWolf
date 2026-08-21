#include "vulkan_command_buffer.hpp"

VkCommandBuffer rw::vk::CommandBuffer::handle() const {
    return command_buffer_;
}

void rw::vk::CommandBuffer::set_state(State state) {
    state_ = state;
}