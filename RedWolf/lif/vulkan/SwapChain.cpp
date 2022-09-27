#include "SwapChain.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

SwapChain::SwapChain(RedWolfManager& manager, PhysicalDevice& physicalDevice, GraphicsDevice& graphicsDevice, Surface& surface) :
   BaseObject(manager), graphicsDevice_{ graphicsDevice }
{
   VkSurfaceCapabilitiesKHR capabilities{ surface.capabilities() };
   VkSurfaceFormatKHR       format{ surface.selectedFormat() };
   VkPresentModeKHR         mode{ surface.selectedMode() };
   VkExtent2D               extent{ surface.selectedExtent() };

   uint32_t imageCount{ capabilities.minImageCount + 1U };
   imageCount = std::clamp(imageCount, 0U, capabilities.maxImageCount);

   VkSwapchainCreateInfoKHR createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
   createInfo.surface = surface.handle();
   createInfo.minImageCount = imageCount;
   createInfo.imageFormat = format.format;
   createInfo.imageColorSpace = format.colorSpace;
   createInfo.imageExtent = extent;
   createInfo.imageArrayLayers = 1U;
   createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

   auto                     queueFamilyIndices{ physicalDevice.availableQueueFamilies() };
   std::array<uint32_t, 2U> indicesArray{ queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].value(),
                                          queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].value() };
   if (
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].value() !=
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].value())
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>(indicesArray.size());
      createInfo.pQueueFamilyIndices = indicesArray.data();
   }
   else
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
   }

   createInfo.preTransform = capabilities.currentTransform;
   createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
   createInfo.presentMode = mode;
   createInfo.clipped = VK_TRUE;

   createInfo.oldSwapchain = VK_NULL_HANDLE; // TODO: Add support for swap chain recreation.

   swapChain_ = vulkanInterface_.createSwapChain(graphicsDevice_.handle(), createInfo);
   if (swapChain_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create swap chain.");
   }
}

SwapChain::~SwapChain()
{
   vulkanInterface_.destroySwapChain(graphicsDevice_.handle(), swapChain_);
}