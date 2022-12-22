#include "SwapChain.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

SwapChain::SwapChain(RedWolfManager& manager, PhysicalDevice& physicalDevice, GraphicsDevice& graphicsDevice, Surface& surface) :
   BaseObject(manager), graphicsDevice_{ graphicsDevice }, format_{ surface.selectedFormat() }, mode_{ surface.selectedMode() }, extent_{
      surface.selectedExtent()
   }
{
   VkSurfaceCapabilitiesKHR capabilities{ surface.capabilities() };

   uint32_t imageCount{ capabilities.minImageCount + 1U };
   imageCount = std::clamp(imageCount, 0U, capabilities.maxImageCount);

   VkSwapchainCreateInfoKHR createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
   createInfo.surface = surface.handle();
   createInfo.minImageCount = imageCount;
   createInfo.imageFormat = format_.format;
   createInfo.imageColorSpace = format_.colorSpace;
   createInfo.imageExtent = extent_;
   createInfo.imageArrayLayers = 1U;
   createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

   auto                     queueFamilyIndices{ physicalDevice.availableQueueFamilies() };
   std::array<uint32_t, 2U> indicesArray{ queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].value(),
                                          queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].value() };
   createInfo.queueFamilyIndexCount = static_cast<uint32_t>(indicesArray.size());
   createInfo.pQueueFamilyIndices = indicesArray.data();

   if (
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].value() !=
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].value())
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
   }
   else
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
   }

   createInfo.preTransform = capabilities.currentTransform;
   createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
   createInfo.presentMode = mode_;
   createInfo.clipped = VK_TRUE;

   createInfo.oldSwapchain = VK_NULL_HANDLE; // TODO: Add support for swap chain recreation.

   swapChain_ = vulkanInterface_.createSwapChain(graphicsDevice_.handle(), createInfo);
   if (swapChain_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create swap chain.");
   }

   // Initialise the swap chain images.
   std::vector<VkImage> imageHandles{ vulkanInterface_.getSwapChainImages(graphicsDevice_.handle(), swapChain_) };
   images_.reserve(imageHandles.size());
   if (images_.empty())
   {
      logger_.relFatal("No images available in the swap chain.");
   }

   for (const auto& imageHandle : imageHandles)
   {
      images_.emplace_back(manager_, graphicsDevice_, imageHandle, format_.format);
   }
}

SwapChain::~SwapChain()
{
   vulkanInterface_.destroySwapChain(graphicsDevice_.handle(), swapChain_);
}

SwapChain::SwapChain(SwapChain&& other) noexcept :
   BaseObject(other.manager_), graphicsDevice_{ other.graphicsDevice_ }, swapChain_{ other.swapChain_ }
{
   other.swapChain_ = VK_NULL_HANDLE;

   images_ = std::move(other.images_);
}

const VkSurfaceFormatKHR& SwapChain::format() const
{
   return format_;
}

const std::vector<ImageView>& SwapChain::images() const
{
   return images_;
}