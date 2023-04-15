#include "SwapChain.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"
#include "RedWolf/lif/vulkan/Fence.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Semaphore.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

SwapChain::SwapChain(
   RedWolfManager&       manager,
   const PhysicalDevice& physicalDevice,
   const DeviceBase&     device,
   const Surface&        surface,
   SwapChain*            oldSwapchain) :
   BaseObject(manager),
   device_{ device }, format_{ surface.selectedFormat() }, mode_{ surface.selectedMode() }, extent_{ surface.selectedExtent() }
{
   VkSurfaceCapabilitiesKHR capabilities{ surface.capabilities() };

   u32 imageCount{ capabilities.minImageCount + 1U };
   if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) imageCount = capabilities.maxImageCount;

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

   if (
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].value() !=
      queueFamilyIndices.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].value())
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = static_cast<u32>(indicesArray.size());
      createInfo.pQueueFamilyIndices = indicesArray.data();
   }
   else
   {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
   }

   createInfo.preTransform = capabilities.currentTransform;
   createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
   createInfo.presentMode = mode_;
   createInfo.clipped = VK_TRUE;

   if (oldSwapchain != nullptr)
   {
      createInfo.oldSwapchain = oldSwapchain->handle();
   }
   else
   {
      createInfo.oldSwapchain = VK_NULL_HANDLE;
   }

   swapChain_ = device_.createSwapchain(createInfo);
   if (swapChain_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create swap chain.");
   }

   // Initialise the swap chain images.
   std::vector<VkImage> imageHandles;
   imageCount = 0U;
   VkResult errCode{ vkGetSwapchainImagesKHR(device_.handle(), swapChain_, &imageCount, nullptr) };
   if (errCode == VK_SUCCESS && imageCount > 0U)
   {
      imageHandles.resize(imageCount);
      errCode = vkGetSwapchainImagesKHR(device_.handle(), swapChain_, &imageCount, imageHandles.data());
   }
   if (imageHandles.empty())
   {
      logger_.relFatal("No images available in the swap chain.");
   }

   images_.reserve(imageHandles.size());
   for (const auto& imageHandle : imageHandles)
   {
      images_.emplace_back(manager_, device_, imageHandle, format_.format);
   }
}

SwapChain::~SwapChain()
{
   device_.destroySwapchain(swapChain_);
}

SwapChain::SwapChain(SwapChain&& other) noexcept : BaseObject(other.manager_), device_{ other.device_ }, swapChain_{ other.swapChain_ }
{
   other.swapChain_ = VK_NULL_HANDLE;

   images_ = std::move(other.images_);
}

u32 SwapChain::acquireNextImage(Semaphore* semaphore, Fence* fence)
{
   u32 result{ 0U };

   VkResult errCode{ vkAcquireNextImageKHR(
      device_.handle(),
      swapChain_,
      std::chrono::nanoseconds(std::numeric_limits<u64>::max()).count(),
      semaphore != nullptr ? semaphore->handle() : VK_NULL_HANDLE,
      fence != nullptr ? fence->handle() : VK_NULL_HANDLE,
      &result) };
   if (errCode != VK_SUCCESS)
   {
      if (errCode == VK_ERROR_OUT_OF_DATE_KHR || errCode == VK_SUBOPTIMAL_KHR) needsRecreation_ = true;
      return 0U;
   }

   return result;
}

const VkSurfaceFormatKHR& SwapChain::format() const
{
   return format_;
}

VkSwapchainKHR SwapChain::handle() const
{
   return swapChain_;
}

const std::vector<ImageView>& SwapChain::images() const
{
   return images_;
}

bool SwapChain::needsRecreation() const
{
   return needsRecreation_;
}