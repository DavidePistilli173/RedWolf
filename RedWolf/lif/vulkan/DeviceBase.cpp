#include "DeviceBase.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"

using namespace rw::lif::vlk;

DeviceBase::~DeviceBase()
{
   if (device_ != VK_NULL_HANDLE) vkDestroyDevice(device_, nullptr);
}

DeviceBase::DeviceBase(DeviceBase&& other) noexcept :
   BaseObject(other.manager_), physicalDevice_{ other.physicalDevice_ }, device_{ other.device_ }, queues_{ other.queues_ },
   memoryAllocator_{ std::move(other.memoryAllocator_) }
{
   other.device_ = VK_NULL_HANDLE;
}

QueueFamilies DeviceBase::availableQueueFamilies() const
{
   return physicalDevice_.availableQueueFamilies();
}

std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo>
   DeviceBase::createBuffer(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) const
{
   return memoryAllocator_->createBuffer(bufferInfo, allocationInfo);
}

VkCommandPool DeviceBase::createCommandPool(const VkCommandPoolCreateInfo& commandPoolInfo) const
{
   VkCommandPool result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateCommandPool(device_, &commandPoolInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create command pool: {}.", errCode);
   }

   return result;
}

VkFence DeviceBase::createFence(const VkFenceCreateInfo& fenceInfo) const
{
   VkFence result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateFence(device_, &fenceInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create fence: {}", errCode);
   }

   return result;
}

VkFramebuffer DeviceBase::createFramebuffer(const VkFramebufferCreateInfo& framebufferInfo) const
{
   VkFramebuffer result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateFramebuffer(device_, &framebufferInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create framebuffer: {}.", errCode);
   }

   return result;
}

VkImageView DeviceBase::createImageView(const VkImageViewCreateInfo& viewInfo) const
{
   VkImageView result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateImageView(device_, &viewInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create image view: {}.", errCode);
   }

   return result;
}

VkPipelineLayout DeviceBase::createPipelineLayout(const VkPipelineLayoutCreateInfo& pipelineLayoutInfo) const
{
   VkPipelineLayout result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreatePipelineLayout(device_, &pipelineLayoutInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create pipeline layout: {}.", errCode);
   }

   return result;
}

VkRenderPass DeviceBase::createRenderPass(const VkRenderPassCreateInfo& renderPassInfo) const
{
   VkRenderPass result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateRenderPass(device_, &renderPassInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create render pass: {}.", errCode);
   }

   return result;
}

VkSemaphore DeviceBase::createSemaphore(const VkSemaphoreCreateInfo& semaphoreInfo) const
{
   VkSemaphore result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create semaphore: {}.", errCode);
   }

   return result;
}

VkShaderModule DeviceBase::createShaderModule(const VkShaderModuleCreateInfo& shaderInfo) const
{
   VkShaderModule result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateShaderModule(device_, &shaderInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create shader module: {}.", errCode);
   }

   return result;
}

VkSwapchainKHR DeviceBase::createSwapchain(const VkSwapchainCreateInfoKHR& swapchainInfo) const
{
   VkSwapchainKHR result{ VK_NULL_HANDLE };

   VkResult errCode{ vkCreateSwapchainKHR(device_, &swapchainInfo, nullptr, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create swap chain: {}.", errCode);
   }

   return result;
}

void DeviceBase::destroyBuffer(VkBuffer buffer, VmaAllocation allocation) const
{
   memoryAllocator_->destroyBuffer(buffer, allocation);
}

void DeviceBase::destroyCommandPool(VkCommandPool commandPool) const
{
   if (commandPool == VK_NULL_HANDLE) return;
   vkDestroyCommandPool(device_, commandPool, nullptr);
}

void DeviceBase::destroyFence(VkFence fence) const
{
   if (fence == VK_NULL_HANDLE) return;
   vkDestroyFence(device_, fence, nullptr);
}

void DeviceBase::destroyFramebuffer(VkFramebuffer framebuffer) const
{
   if (framebuffer == VK_NULL_HANDLE) return;
   vkDestroyFramebuffer(device_, framebuffer, nullptr);
}

void DeviceBase::destroyImageView(VkImageView imageView) const
{
   if (imageView == VK_NULL_HANDLE) return;
   vkDestroyImageView(device_, imageView, nullptr);
}

void DeviceBase::destroyPipeline(VkPipeline pipeline) const
{
   if (pipeline == VK_NULL_HANDLE) return;
   vkDestroyPipeline(device_, pipeline, nullptr);
}

void DeviceBase::destroyPipelineLayout(VkPipelineLayout pipelineLayout) const
{
   if (pipelineLayout == VK_NULL_HANDLE) return;
   vkDestroyPipelineLayout(device_, pipelineLayout, nullptr);
}

void DeviceBase::destroyRenderPass(VkRenderPass renderPass) const
{
   if (renderPass == VK_NULL_HANDLE) return;
   vkDestroyRenderPass(device_, renderPass, nullptr);
}

void DeviceBase::destroySemaphore(VkSemaphore semaphore) const
{
   if (semaphore == VK_NULL_HANDLE) return;
   vkDestroySemaphore(device_, semaphore, nullptr);
}

void DeviceBase::destroyShaderModule(VkShaderModule shaderModule) const
{
   if (shaderModule == VK_NULL_HANDLE) return;
   vkDestroyShaderModule(device_, shaderModule, nullptr);
}

void DeviceBase::destroySwapchain(VkSwapchainKHR swapchain) const
{
   if (swapchain == VK_NULL_HANDLE) return;
   vkDestroySwapchainKHR(device_, swapchain, nullptr);
}

VkDevice DeviceBase::handle() const
{
   return device_;
}

void* DeviceBase::mapMemory(VmaAllocation allocation) const
{
   return memoryAllocator_->mapMemory(allocation);
}

VkQueue DeviceBase::queue(QueueFamilies::Id queueId) const
{
   return queues_[static_cast<size_t>(queueId)];
}

void DeviceBase::unmapMemory(VmaAllocation allocation) const
{
   memoryAllocator_->unmapMemory(allocation);
}

void DeviceBase::waitIdle()
{
   vkDeviceWaitIdle(device_);
}