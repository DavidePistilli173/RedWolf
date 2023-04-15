#include "FrameBuffer.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"
#include "RedWolf/lif/vulkan/ImageView.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

FrameBuffer::FrameBuffer(
   RedWolfManager& manager, const DeviceBase& device, const RenderPass& renderPass, const ImageView& imageView, const Surface& surface) :
   BaseObject(manager),
   device_{ device }, extent_{ surface.selectedExtent() }
{
   VkImageView attachments[] = { imageView.handle() };

   VkFramebufferCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
   createInfo.renderPass = renderPass.handle();
   createInfo.attachmentCount = 1U;
   createInfo.pAttachments = attachments;
   createInfo.width = extent_.width;
   createInfo.height = extent_.height;
   createInfo.layers = 1U;

   framebuffer_ = device_.createFramebuffer(createInfo);
   if (framebuffer_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create framebuffer.");
   }
}

FrameBuffer::~FrameBuffer()
{
   device_.destroyFramebuffer(framebuffer_);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
   BaseObject(other.manager_), framebuffer_{ other.framebuffer_ }, device_{ other.device_ }, extent_{ other.extent_ }
{
   other.framebuffer_ = VK_NULL_HANDLE;
}

VkExtent2D FrameBuffer::extent() const
{
   return extent_;
}

VkFramebuffer FrameBuffer::handle() const
{
   return framebuffer_;
}