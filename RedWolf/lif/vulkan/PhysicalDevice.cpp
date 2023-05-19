#include "PhysicalDevice.hpp"

using namespace rw::lif::vulkan;

PhysicalDevice::PhysicalDevice(rw::util::Logger& logger, VkPhysicalDevice physicalDevice) :
   logger_{ logger }, physicalDevice_{ physicalDevice }
{
   if (physicalDevice_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Null physical device handle.");
   }

   // Initialise physical device properties.
   vkGetPhysicalDeviceProperties(physicalDevice_, &properties_);
   vkGetPhysicalDeviceFeatures(physicalDevice_, &features_);
   vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memoryProperties_);

   // Initialise supported queues.
   u32 queueFamilyCount{ 0U };
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
   queueFamilyProperties_.resize(queueFamilyCount);
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilyProperties_.data());
}