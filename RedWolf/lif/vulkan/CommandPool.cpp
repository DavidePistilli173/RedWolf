#include "CommandPool.hpp"

using namespace rw::lif::vlk;

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

CommandPool::CommandPool(RedWolfManager& manager, GraphicsDevice& device, QueueFamilies::Id queueType) :
   BaseObject(manager), graphicsDevice_{ device }
{
   VkCommandPoolCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
   createInfo.queueFamilyIndex = graphicsDevice_.availableQueueFamilies().queues[static_cast<size_t>(queueType)].value();

   commandPool_ = vulkanInterface_.createCommandPool(graphicsDevice_.handle(), createInfo);
   if (commandPool_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create command pool.");
   }
}

CommandPool::~CommandPool()
{
   vulkanInterface_.destroyCommandPool(graphicsDevice_.handle(), commandPool_);
}

CommandPool::CommandPool(CommandPool&& other) noexcept :
   BaseObject(other.manager_), commandPool_{ other.commandPool_ }, graphicsDevice_{ other.graphicsDevice_ }
{
}

VkCommandPool CommandPool::handle() const
{
   return commandPool_;
}