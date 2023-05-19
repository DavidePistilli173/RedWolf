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

   u32      count{ 0U };
   VkResult errCode{ vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &count, nullptr) };
   if (errCode == VK_SUCCESS && count > 0U)
   {
      extensionProperties_.resize(count);
      errCode = vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &count, extensionProperties_.data());
      if (errCode != VK_SUCCESS)
      {
         extensionProperties_.resize(0U);
      }
   }

   // Initialise supported queues.
   u32 queueFamilyCount{ 0U };
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
   queueFamilyProperties_.resize(queueFamilyCount);
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilyProperties_.data());
}

const std::vector<VkExtensionProperties>& PhysicalDevice::extensionProperties() const
{
   return extensionProperties_;
}

const VkPhysicalDeviceFeatures& PhysicalDevice::features() const
{
   return features_;
}

VkPhysicalDevice PhysicalDevice::handle() const
{
   return physicalDevice_;
}

const VkPhysicalDeviceMemoryProperties& PhysicalDevice::memoryProperties() const
{
   return memoryProperties_;
}

const VkPhysicalDeviceProperties& PhysicalDevice::properties() const
{
   return properties_;
}

const std::vector<VkQueueFamilyProperties> PhysicalDevice::queueFamilyProperties() const
{
   return queueFamilyProperties_;
}