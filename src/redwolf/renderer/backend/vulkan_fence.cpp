#include "vulkan_fence.hpp"

#include "vulkan_common.hpp"

rw::vk::Fence::~Fence() {
    vkDestroyFence(device_->logical(), fence_, allocator_);
}

VkFence rw::vk::Fence::handle() const {
    return fence_;
}

bool rw::vk::Fence::init(VkAllocationCallbacks* allocator, Ptr<Device> device, bool starts_signaled) {
    allocator_ = allocator;
    device_    = std::move(device);
    signaled_  = starts_signaled;

    const VkFenceCreateInfo fence_create_info{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = nullptr, .flags = 0 };
    RW_VK_CHECK(vkCreateFence(device_->logical(), &fence_create_info, allocator_, &fence_), "Failed to create fence: '{}'", false)

    return true;
}

bool rw::vk::Fence::reset() {
    if (signaled_) {
        RW_VK_CHECK(vkResetFences(device_->logical(), 1, &fence_), "Failed to reset fence: '{}'", false)
        signaled_ = false;
    }

    return true;
}

bool rw::vk::Fence::wait(std::chrono::nanoseconds timeout) {
    if (!signaled_) {
        const VkResult res{ vkWaitForFences(device_->logical(), 1, &fence_, VK_TRUE, timeout.count()) };
        switch (res) {
        case VK_SUCCESS:
            signaled_ = true;
            return true;
        case VK_TIMEOUT:
            warn("Fence timed out.");
            return false;
        default:
            error("Fence wait error: '{}'", string_VkResult(res));
            return false;
        }
    }

    // Fence already signalled, do not wait.
    return true;
}