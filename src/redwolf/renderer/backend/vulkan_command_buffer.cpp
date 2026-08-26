#include "vulkan_command_buffer.hpp"

#include "vulkan_common.hpp"

#include <vulkan/vulkan_core.h>

rw::vk::CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(device_->logical(), command_pool_, 1, &command_buffer_);
}

bool rw::vk::CommandBuffer::begin(bool is_single_use, bool is_renderpass_continue, bool is_simultaneous_use) {
    VkCommandBufferBeginInfo begin_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .pNext = nullptr, .flags = 0, .pInheritanceInfo = nullptr
    };

    if (is_single_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }

    if (is_renderpass_continue) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }

    if (is_simultaneous_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    RW_VK_CHECK(vkBeginCommandBuffer(command_buffer_, &begin_info), "Failed to begin command buffer: '{}'", false)
    state_ = State::recording;

    return true;
}

bool rw::vk::CommandBuffer::end() {
    RW_VK_CHECK(vkEndCommandBuffer(command_buffer_), "Failed to end command buffer: '{}'", false)
    state_ = State::recording_ended;
    return true;
}

bool rw::vk::CommandBuffer::end_and_submit_single_use(VkQueue queue) {
    if (!end()) {
        error("Failed to end command buffer.");
        return false;
    }

    // Submit it to the queue.
    const VkSubmitInfo submit_info{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .commandBufferCount = 1, .pCommandBuffers = &command_buffer_ };
    RW_VK_CHECK(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE), "Failed to submit command buffer: '{}'", false)

    // Wait for it to finish.
    RW_VK_CHECK(vkQueueWaitIdle(queue), "Failed to wait for the queue: '{}'", false)

    return true;
}

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
        vkAllocateCommandBuffers(device_->logical(), &allocate_info, &command_buffer_), "Failed to create command buffer: '{}'", false)
    state_ = State::ready;
    return true;
}

bool rw::vk::CommandBuffer::init_and_begin_single_use(Ptr<Device> device, VkCommandPool command_pool) {
    if (!init(std::move(device), command_pool, true)) {
        error("Failed to initialise command buffer.");
        return false;
    }

    if (!begin(true, false, false)) {
        error("Failed to begin command buffer.");
        return false;
    }

    return true;
}

void rw::vk::CommandBuffer::set_state(State state) {
    state_ = state;
}

void rw::vk::CommandBuffer::reset() {
    state_ = State::ready;
}

void rw::vk::CommandBuffer::update_submitted() {
    state_ = State::submitted;
}
