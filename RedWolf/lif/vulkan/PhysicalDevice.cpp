#include "PhysicalDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

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
         queueFamilyProperties_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
         queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::graphics)] = i;
      }
   }
}

rw::lif::vlk::QueueFamilies PhysicalDevice::availableQueueFamilies() const
{
   std::scoped_lock lck{ mtx_ };
   return queueFamilies_;
}

std::unique_ptr<GraphicsDevice> PhysicalDevice::createGraphicsDevice(VkSurfaceKHR surface)
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
   std::scoped_lock lck{ mtx_ };

   if (queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::presentation)].has_value()) return true;

   for (uint32_t i{ 0U }; i < queueFamilyProperties_.size(); ++i)
   {
      if (vulkanInterface_.checkSurfaceSupport(device_, i, surface))
      {
         queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::presentation)] = i;
         return true;
      }
   }

   return false;
}