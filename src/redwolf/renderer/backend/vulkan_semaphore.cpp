#include "vulkan_semaphore.hpp"

#include "vulkan_common.hpp"

rw::vk::Semaphore::~Semaphore() {
    vkDestroySemaphore(device_->logical(), semaphore_, allocator_);
}

VkSemaphore rw::vk::Semaphore::handle() const {
    return semaphore_;
}

VkSemaphore* rw::vk::Semaphore::handle_pointer() {
    return &semaphore_;
}

bool rw::vk::Semaphore::init(VkAllocationCallbacks* allocator, Ptr<Device> device) {
    allocator_ = allocator;
    device_    = std::move(device);

    const VkSemaphoreCreateInfo semaphore_create_info{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = nullptr, .flags = 0 };
    RW_VK_CHECK(
        vkCreateSemaphore(device_->logical(), &semaphore_create_info, allocator_, &semaphore_), "Failed to create semaphore: '{}'", false)

    return true;
}