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

   vkGetPhysicalDeviceProperties(device_, &properties_);
   vkGetPhysicalDeviceFeatures(device_, &features_);
   vkGetPhysicalDeviceMemoryProperties(device_, &memoryProperties_);

   u32 queueFamilyCount{ 0U };
   vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, nullptr);
   queueFamilyProperties_.resize(queueFamilyCount);
   vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, queueFamilyProperties_.data());

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

VkDevice PhysicalDevice::createDevice(const VkDeviceCreateInfo& deviceInfo) const
{
   VkDevice result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateDevice(device_, &deviceInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create logical device: {}", errCode);
   }

   return result;
}

std::unique_ptr<GraphicsDevice> PhysicalDevice::createGraphicsDevice() const
{
   return std::make_unique<GraphicsDevice>(manager_, *this);
}

VkPhysicalDevice PhysicalDevice::handle() const
{
   return device_;
}

const VkPhysicalDeviceMemoryProperties& PhysicalDevice::memoryProperties() const
{
   return memoryProperties_;
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
      u32      i{ 0U };
      VkBool32 surfaceSupported{ VK_FALSE };
      while (i < queueFamilyProperties_.size() && surfaceSupported == VK_FALSE)
      {
         VkResult errCode{ vkGetPhysicalDeviceSurfaceSupportKHR(device_, i, surface, &surfaceSupported) };
         if (errCode != VK_SUCCESS)
         {
            logger_.relWarn("Failed to check surface support: {}", errCode);
         }
         if (surfaceSupported == VK_FALSE) ++i;
      }

      if (i >= queueFamilyProperties_.size())
      {
         return false;
      }
      queueFamilies_.queues[static_cast<size_t>(QueueFamilies::Id::presentation)] = i;
   }

   // Swap chain extension.
   std::vector<VkExtensionProperties> supportedExtensions;

   u32      count{ 0U };
   VkResult errCode{ vkEnumerateDeviceExtensionProperties(device_, nullptr, &count, nullptr) };

   if (errCode == VK_SUCCESS && count > 0U)
   {
      supportedExtensions.resize(count);
      errCode = vkEnumerateDeviceExtensionProperties(device_, nullptr, &count, supportedExtensions.data());
      if (errCode != VK_SUCCESS)
      {
         supportedExtensions.resize(0U);
      }
   }

   return std::any_of(
      supportedExtensions.begin(),
      supportedExtensions.end(),
      [](const VkExtensionProperties& elem) { return strcmp(&elem.extensionName[0], VK_KHR_SWAPCHAIN_EXTENSION_NAME); });
}

const VkPhysicalDeviceProperties& PhysicalDevice::properties() const
{
   return properties_;
}