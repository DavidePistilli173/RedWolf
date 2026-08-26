#include "vulkan_framebuffer.hpp"

#include "vulkan_common.hpp"

rw::vk::Framebuffer::~Framebuffer() {
    vkDestroyFramebuffer(device_->logical(), framebuffer_, allocator_);
}

VkFramebuffer rw::vk::Framebuffer::handle() const {
    return framebuffer_;
}

bool rw::vk::Framebuffer::init(const Params& params) {
    allocator_   = params.allocator;
    device_      = params.device;
    attachments_ = params.attachments.clone();
    render_pass_ = params.render_pass;

    const VkFramebufferCreateInfo framebuffer_create_info{ .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                                                           .pNext           = nullptr,
                                                           .flags           = 0,
                                                           .renderPass      = render_pass_->handle(),
                                                           .attachmentCount = static_cast<u32>(attachments_.size()),
                                                           .pAttachments    = attachments_.data(),
                                                           .width           = params.width,
                                                           .height          = params.height,
                                                           .layers          = 1 };

    RW_VK_CHECK(
        vkCreateFramebuffer(device_->logical(), &framebuffer_create_info, allocator_, &framebuffer_),
        "Failed to create framebuffer: '{}'",
        false)

    return true;
}