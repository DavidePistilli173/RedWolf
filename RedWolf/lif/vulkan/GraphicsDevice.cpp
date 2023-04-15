#include "GraphicsDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

GraphicsDevice::GraphicsDevice(RedWolfManager& manager, const PhysicalDevice& physicalDevice) :
   DeviceBase(manager, physicalDevice, required_queues, required_extensions)
{
}

GraphicsDevice::GraphicsDevice(GraphicsDevice&& other) noexcept :
   DeviceBase(other.manager_, other.physicalDevice_, required_queues, required_extensions)
{
}

VkPipeline GraphicsDevice::createGraphicsPipeline(const VkGraphicsPipelineCreateInfo& pipelineInfo) const
{
   VkPipeline result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1U, &pipelineInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create graphics pipeline: {}.", errCode);
   }

   return result;
}

VkQueue GraphicsDevice::graphicsQueue() const
{
   return queues_[static_cast<size_t>(QueueFamilies::Id::graphics)];
}

VkQueue GraphicsDevice::presentationQueue() const
{
   return queues_[static_cast<size_t>(QueueFamilies::Id::presentation)];
}