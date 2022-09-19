#include "PhysicalDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

PhysicalDevice::PhysicalDevice(RedWolfManager& manager, VkPhysicalDevice device) :
   logger_{ manager.logger() }, vulkanInterface_{ manager.vulkanInterface() }, device_{ device }
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relFatal("Null device handle.");
   }

   auto data{ vulkanInterface_.getPhysicalDeviceData(device_) };
   properties_ = data.first;
   features_ = data.second;

   auto queueFamilyProperties_ = vulkanInterface_.getPhysicalDeviceQueueFamilies(device_);
   for (size_t i{ 0U }; i < queueFamilyProperties_.size(); ++i)
   {
      if (!queueFamilies_.graphics.has_value() && queueFamilyProperties_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
         queueFamilies_.graphics = i;
      }
   }
}

rw::lif::vlk::QueueFamilies PhysicalDevice::availableQueueFamilies() const
{
   std::scoped_lock lck{ mtx_ };
   return queueFamilies_;
}

VkPhysicalDevice PhysicalDevice::handle()
{
   return device_;
}

bool PhysicalDevice::isOpCategorySupported(OpCategory cat) const
{
   return queueFamilies_.graphics.has_value();
}

bool PhysicalDevice::isSurfaceSupported(VkSurfaceKHR surface)
{
   std::scoped_lock lck{ mtx_ };

   if (queueFamilies_.presentation.has_value()) return true;

   for (size_t i{ 0U }; i < queueFamilyProperties_.size(); ++i)
   {
      if (vulkanInterface_.checkSurfaceSupport(device_, i, surface))
      {
         queueFamilies_.presentation = i;
         return true;
      }
   }

   return false;
}