#include "PhysicalDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

#include <algorithm>

using namespace rw::lif::vlk;

PhysicalDevice::PhysicalDevice(RedWolfManager& manager, VkPhysicalDevice device) : BaseObject(manager), device_{ device }
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relFatal("Null device handle.");
   }

   auto data{ vulkanInterface_.getPhysicalDeviceData(device_) };
   properties_ = data.first;
   features_ = data.second;

   queueFamilyProperties_ = vulkanInterface_.getPhysicalDeviceQueueFamilies(device_);
   for (size_t i{ 0U }; i < queueFamilyProperties_.size(); ++i)
   {
      if (
         !queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].has_value() &&
         static_cast<bool>(queueFamilyProperties_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
      {
         queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::graphics)] = i;
      }
   }
}

rw::lif::vlk::QueueFamilies PhysicalDevice::availableQueueFamilies() const
{
   const std::scoped_lock lck{ mtx_ };
   return queueFamilies_;
}

std::unique_ptr<GraphicsDevice> PhysicalDevice::createGraphicsDevice()
{
   return std::make_unique<GraphicsDevice>(manager_, *this);
}

VkPhysicalDevice PhysicalDevice::handle()
{
   return device_;
}

bool PhysicalDevice::isOpCategorySupported(OpCategory cat) const
{
   switch (cat)
   {
   case OpCategory::graphics:
      return queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::graphics)].has_value();
      break;
   }

   return false;
}

bool PhysicalDevice::isSurfaceSupported(VkSurfaceKHR surface)
{
   const std::scoped_lock lck{ mtx_ };

   if (!queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].has_value())
   {
      uint32_t i{ 0U };
      while (i < queueFamilyProperties_.size() && !vulkanInterface_.checkSurfaceSupport(device_, i, surface))
      {
         ++i;
      }

      if (i >= queueFamilyProperties_.size())
      {
         return false;
      }
      queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::presentation)] = i;
   }

   // Swap chain extension.
   std::vector<VkExtensionProperties> supportedExtensions{ vulkanInterface_.enumerateDeviceExtensionProperties(device_) };

   return std::any_of(
      supportedExtensions.begin(),
      supportedExtensions.end(),
      [](const VkExtensionProperties& elem) { return strcmp(&elem.extensionName[0], VK_KHR_SWAPCHAIN_EXTENSION_NAME); });
}