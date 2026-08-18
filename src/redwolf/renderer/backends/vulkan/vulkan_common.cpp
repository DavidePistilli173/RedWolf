#include "vulkan_common.hpp"

namespace {
    rw::VulkanContext* vk_context_{ nullptr };
}

rw::VulkanContext& rw::VulkanContext::ctx() {
    return *vk_context_;
}

void rw::VulkanContext::init() {
    if (nullptr != vk_context_) {
        warn("Vulkan context already initialised.");
        return;
    }

    vk_context_ = new VulkanContext();
}

void rw::VulkanContext::shutdown() {
    if (nullptr == vk_context_) {
        warn("Vulkan context already shut down.");
        return;
    }

    delete vk_context_;
}