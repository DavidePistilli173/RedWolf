#include "Surface.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"

#include <algorithm>
#include <array>

using namespace rw::lif::vlk;

Surface::Surface(RedWolfManager& manager, GLFWwindow* window) :
   BaseObject(manager), glfwManager_{ manager.glfwManager() }, window_{ window }, surface_{ glfwManager_.createWindowSurface(
                                                                                     vulkanInstance_.handle(), window_) }
{
   if (surface_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create window surface.");
   }
}

Surface::~Surface()
{
   swapChain_.reset();
   vulkanInstance_.destroySurface(surface_);
}

VkSurfaceCapabilitiesKHR Surface::capabilities() const
{
   const std::shared_lock lck{ mtx_ };
   return capabilities_;
}

std::vector<VkSurfaceFormatKHR> Surface::formats() const
{
   const std::shared_lock lck{ mtx_ };
   return formats_;
}

VkSurfaceKHR Surface::handle() const
{
   return surface_;
}

std::vector<VkPresentModeKHR> Surface::modes() const
{
   const std::shared_lock lck{ mtx_ };
   return modes_;
}

[[nodiscard]] VkExtent2D Surface::selectedExtent() const
{
   const std::shared_lock lck{ mtx_ };
   return selectedExtent_;
}

[[nodiscard]] VkSurfaceFormatKHR Surface::selectedFormat() const
{
   const std::shared_lock lck{ mtx_ };
   return selectedFormat_;
}

[[nodiscard]] VkPresentModeKHR Surface::selectedMode() const
{
   const std::shared_lock lck{ mtx_ };
   return selectedMode_;
}

bool Surface::setDevices(const PhysicalDevice& physicalDevice, const GraphicsDevice& graphicsDevice)
{
   if (!initDevices_(physicalDevice)) return false;
   return initSwapChain_(physicalDevice, graphicsDevice);
}

SwapChain* Surface::swapChain() const
{
   return swapChain_.get();
}

bool Surface::initDevices_(const PhysicalDevice& physicalDevice)
{
   const std::scoped_lock lck{ mtx_ };

   VkResult errCode{ vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.handle(), surface_, &capabilities_) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relFatal("Failed to get surface capabilities: {}.", errCode);
   }

   u32 formatCount{ 0U };
   errCode = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.handle(), surface_, &formatCount, nullptr);
   if (errCode == VK_SUCCESS && formatCount > 0U)
   {
      formats_.resize(formatCount);
      errCode = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.handle(), surface_, &formatCount, formats_.data());
      if (errCode != VK_SUCCESS)
      {
         formats_.resize(0U);
      }
   }

   u32 modeCount{ 0U };
   errCode = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.handle(), surface_, &modeCount, nullptr);
   if (errCode == VK_SUCCESS && modeCount > 0U)
   {
      modes_.resize(modeCount);
      errCode = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.handle(), surface_, &modeCount, modes_.data());
   }

   if (formats_.empty() || modes_.empty())
   {
      logger_.relErr("The current physical device does not support all features required by the used surface.");
      return false;
   }

   // Compute the best image format.
   bool   done{ false };
   size_t i{ 0U };
   while (i < formats_.size() && !done)
   {
      if (formats_[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats_[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
         selectedFormat_ = formats_[i];
         done = true;
      }
      ++i;
   }
   if (!done) selectedFormat_ = formats_[0];

   // Compute the best presentation mode.
   done = false;
   i = 0U;
   while (i < modes_.size() && !done)
   {
      if (modes_[i] == VK_PRESENT_MODE_FIFO_KHR)
      {
         selectedMode_ = modes_[i];
         done = true;
      }
      ++i;
   }
   if (!done) selectedMode_ = VK_PRESENT_MODE_IMMEDIATE_KHR;

   // Compute the surface resolution.
   auto [width, height] = glfwManager_.getFrameBufferSize(window_);
   selectedExtent_ = { static_cast<u32>(width), static_cast<u32>(height) };
   selectedExtent_.width = std::clamp(selectedExtent_.width, capabilities_.minImageExtent.width, capabilities_.maxImageExtent.width);
   selectedExtent_.height = std::clamp(selectedExtent_.height, capabilities_.minImageExtent.height, capabilities_.maxImageExtent.height);
}

bool Surface::initSwapChain_(const PhysicalDevice& physicalDevice, const GraphicsDevice& graphicsDevice)
{
   swapChain_ = std::make_unique<SwapChain>(manager_, physicalDevice, graphicsDevice, *this, swapChain_.get());
   return true;
}