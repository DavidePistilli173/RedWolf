#include "vulkan_command_buffer.hpp"

#include "vulkan_common.hpp"

#include <vulkan/vulkan_core.h>

VkCommandBuffer rw::vk::CommandBuffer::handle() const {
    return command_buffer_;
}

bool rw::vk::CommandBuffer::init(Ptr<Device> device, VkCommandPool command_pool, bool is_primary) {
    device_ = std::move(device);

    const VkCommandBufferAllocateInfo allocate_info{ .sType       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                     .pNext       = nullptr,
                                                     .commandPool = command_pool,
                                                     .level =
                                                         is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY,
                                                     .commandBufferCount = 1 };

    RW_VK_CHECK(
        vkAllocateCommandBuffers(device->logical(), &allocate_info, &command_buffer_), "Failed to create command buffer: '{}'", false)
}

void rw::vk::CommandBuffer::set_state(State state) {
    state_ = state;
}