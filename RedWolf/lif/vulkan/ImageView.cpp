#include "ImageView.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"

using namespace rw::lif::vlk;

ImageView::ImageView(RedWolfManager& manager, GraphicsDevice& graphicsDevice, VkImage image, VkFormat format) :
   BaseObject(manager), device_{ graphicsDevice.handle() }, image_{ image }
{
   if (image_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Cannot create an image view from a null image handle.");
   }

   VkImageViewCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   createInfo.image = image;
   createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
   createInfo.format = format;
   createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
   createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
   createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
   createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
   createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   createInfo.subresourceRange.baseMipLevel = 0;
   createInfo.subresourceRange.levelCount = 1;
   createInfo.subresourceRange.baseArrayLayer = 0;
   createInfo.subresourceRange.layerCount = 1;

   view_ = vulkanInterface_.createImageView(device_, createInfo);
   if (view_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create image view.");
   }
}

ImageView::~ImageView()
{
   vulkanInterface_.destroyImageView(device_, view_);
}

ImageView::ImageView(ImageView&& other) noexcept : BaseObject(other.manager_), device_{ other.device_ }, image_{ other.image_ }
{
   view_ = other.view_;
   other.view_ = VK_NULL_HANDLE;
}

VkImageView ImageView::handle()
{
   return view_;
}