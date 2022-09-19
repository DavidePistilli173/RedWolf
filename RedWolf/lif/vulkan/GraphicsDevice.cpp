#include "GraphicsDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

#include <set>

using namespace rw::lif::vlk;

GraphicsDevice::GraphicsDevice(RedWolfManager& manager, PhysicalDevice& physicalDevice, Surface& surface) :
   logger_{ manager.logger() }, vulkanInterface_{ manager.vulkanInterface() }, physicalDevice_{ physicalDevice.handle() }
{
   // Queues.
   QueueFamilies                        availableQueueFamilies{ physicalDevice.availableQueueFamilies() };
   std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{ initialiseQueueData_(availableQueueFamilies) };

   // Device features.
   VkPhysicalDeviceFeatures deviceFeatures{};

   // Create the logical device.
   VkDeviceCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
   createInfo.pQueueCreateInfos = queueCreateInfos.data();
   createInfo.queueCreateInfoCount = queueCreateInfos.size();
   createInfo.pEnabledFeatures = &deviceFeatures;

   device_ = vulkanInterface_.createDevice(physicalDevice_, createInfo);
   if (device_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create device.");
   }

   graphicsQueue_ = vulkanInterface_.getDeviceQueue(device_, availableQueueFamilies.graphics.value());
   if (graphicsQueue_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to get the graphics queue handle.");
   }

   presentationQueue_ = vulkanInterface_.getDeviceQueue(device_, availableQueueFamilies.presentation.value());
   if (presentationQueue_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to get the presentation queue handle.");
   }
}

GraphicsDevice::~GraphicsDevice()
{
   vulkanInterface_.destroyDevice(device_);
}

GraphicsDevice::GraphicsDevice(GraphicsDevice& other) :
   logger_{ other.logger_ }, vulkanInterface_{ other.vulkanInterface_ }, physicalDevice_{ other.physicalDevice_ }
{
   device_ = other.device_;
   other.device_ = VK_NULL_HANDLE;

   graphicsQueue_ = other.graphicsQueue_;
   other.graphicsQueue_ = VK_NULL_HANDLE;

   presentationQueue_ = other.presentationQueue_;
   other.presentationQueue_ = VK_NULL_HANDLE;
}

GraphicsDevice& GraphicsDevice::operator=(GraphicsDevice&& other)
{
   if (physicalDevice_ != VK_NULL_HANDLE)
   {
      vulkanInterface_.destroyDevice(device_);
   }

   physicalDevice_ = other.physicalDevice_;

   device_ = other.device_;
   other.device_ = VK_NULL_HANDLE;

   graphicsQueue_ = other.graphicsQueue_;
   other.graphicsQueue_ = VK_NULL_HANDLE;

   presentationQueue_ = other.presentationQueue_;
   other.presentationQueue_ = VK_NULL_HANDLE;

   return *this;
}

std::vector<VkDeviceQueueCreateInfo> GraphicsDevice::initialiseQueueData_(const QueueFamilies& availableQueueFamilies)
{
   // If the base physical device does not support all the required queues, throw a fatal error.
   if (!availableQueueFamilies.graphics.has_value())
   {
      logger_.relFatal("Vulkan device does not support the graphics pipeline.");
   }
   if (!availableQueueFamilies.presentation.has_value())
   {
      logger_.relFatal("Vulkan device does not support drawing on the target surface.");
   }

   // Required queues.
   std::set<uint32_t> queueFamilyIndices;
   queueFamilyIndices.insert(availableQueueFamilies.graphics.value());
   queueFamilyIndices.insert(availableQueueFamilies.presentation.value());

   std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

   // Common parameters.
   VkDeviceQueueCreateInfo queueCreateInfo{};
   queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   float queuePriority{ 1.0F };
   queueCreateInfo.pQueuePriorities = &queuePriority;
   queueCreateInfo.queueCount = 1U;

   // Add all required queue info.
   for (auto index : queueFamilyIndices)
   {
      queueCreateInfo.queueFamilyIndex = index;
      queueCreateInfos.emplace_back(queueCreateInfo);
   }

   return queueCreateInfos;
}