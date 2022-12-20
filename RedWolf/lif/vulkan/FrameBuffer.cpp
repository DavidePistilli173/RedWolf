#include "FrameBuffer.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/ImageView.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

FrameBuffer::FrameBuffer(
   RedWolfManager& manager, GraphicsDevice& graphicsDevice, RenderPass& renderPass, ImageView& imageView, Surface& surface) :
   BaseObject(manager),
   device_{ graphicsDevice.handle() }
{
   VkImageView attachments[] = { imageView.handle() };
   auto [imageWidth, imageHeight] = surface.selectedExtent();

   VkFramebufferCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
   createInfo.renderPass = renderPass.handle();
   createInfo.attachmentCount = 1U;
   createInfo.pAttachments = attachments;
   createInfo.width = imageWidth;
   createInfo.height = imageHeight;
   createInfo.layers = 1U;

   framebuffer_ = vulkanInterface_.createFramebuffer(device_, createInfo);
   if (framebuffer_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create framebuffer.");
   }
}

FrameBuffer::~FrameBuffer()
{
   vulkanInterface_.destroyFramebuffer(device_, framebuffer_);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
   BaseObject(other.manager_), framebuffer_{ other.framebuffer_ }, device_{ other.device_ }
{
   other.framebuffer_ = VK_NULL_HANDLE;
}