#include "ImageView.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

using namespace rw::lif::vlk;

ImageView::ImageView(RedWolfManager& manager, GraphicsDevice& graphicsDevice, VkImage image) : BaseObject(manager)
{
   if (image == VK_NULL_HANDLE)
   {
      logger_.relFatal("");
   }
}

VkImageView ImageView::handle()
{
   return view_;
}