#include "CommandPool.hpp"

using namespace rw::lif::vlk;

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"

CommandPool::CommandPool(RedWolfManager& manager, const DeviceBase& device, QueueFamilies::Id queueType) :
   BaseObject(manager), device_{ device }
{
   VkCommandPoolCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
   createInfo.queueFamilyIndex = device_.availableQueueFamilies().queues[static_cast<size_t>(queueType)].value();

   commandPool_ = device_.createCommandPool(createInfo);
   if (commandPool_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create command pool.");
   }
}

CommandPool::~CommandPool()
{
   device_.destroyCommandPool(commandPool_);
}

CommandPool::CommandPool(CommandPool&& other) noexcept :
   BaseObject(other.manager_), commandPool_{ other.commandPool_ }, device_{ other.device_ }
{
}

VkCommandBuffer CommandPool::createCommandBuffer(const VkCommandBufferAllocateInfo& commandBufferInfo) const
{
   VkCommandBuffer result{ VK_NULL_HANDLE };

   VkResult errCode{ vkAllocateCommandBuffers(device_.handle(), &commandBufferInfo, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create command buffer: {}", errCode);
   }

   return result;
}

VkCommandPool CommandPool::handle() const
{
   return commandPool_;
}