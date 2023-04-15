#include "Allocator.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vma;

Allocator::Allocator(RedWolfManager& manager, VkPhysicalDevice physicalDevice, VkDevice device) :
   logger_{ manager.logger() }, instance_{ manager.vulkanInstance().handle() }, physicalDevice_{ physicalDevice }, device_{ device }
{
   if (instance_ == VK_NULL_HANDLE || physicalDevice_ == VK_NULL_HANDLE || device_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Cannot create a memory allocator for a null instance, physical device or logical device.");
   }

   VmaVulkanFunctions vulkanFunctions{};
   vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
   vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

   VmaAllocatorCreateInfo allocatorInfo{};
   allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
   allocatorInfo.instance = instance_;
   allocatorInfo.physicalDevice = physicalDevice_;
   allocatorInfo.device = device_;
   allocatorInfo.pVulkanFunctions = &vulkanFunctions;

   VkResult errCode{ vmaCreateAllocator(&allocatorInfo, &allocator_) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relFatal("Failed to create allocator: {}.", errCode);
   }
}

Allocator ::~Allocator()
{
   if (allocator_ != VK_NULL_HANDLE) vmaDestroyAllocator(allocator_);
}

Allocator::Allocator(Allocator&& other) noexcept :
   allocator_{ other.allocator_ }, logger_{ other.logger_ }, instance_{ other.instance_ },
   physicalDevice_{ other.physicalDevice_ }, device_{ other.device_ }
{
   other.allocator_ = VK_NULL_HANDLE;
}

std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo>
   Allocator::createBuffer(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) const
{
   std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo> result{ VK_NULL_HANDLE, VK_NULL_HANDLE, VmaAllocationInfo{} };

   VkResult errCode{ vmaCreateBuffer(
      allocator_, &bufferInfo, &allocationInfo, &std::get<0>(result), &std::get<1>(result), &std::get<2>(result)) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to create buffer: {}.", errCode);
   }

   return result;
}

void Allocator::destroyBuffer(VkBuffer buffer, VmaAllocation allocation) const
{
   if (buffer != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE) vmaDestroyBuffer(allocator_, buffer, allocation);
}

void* Allocator::mapMemory(VmaAllocation allocation) const
{
   if (allocation == VK_NULL_HANDLE) return nullptr;

   void*    result{ nullptr };
   VkResult errCode{ vmaMapMemory(allocator_, allocation, &result) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to map buffer memory: {}.", errCode);
   }

   return result;
}

void Allocator::unmapMemory(VmaAllocation allocation) const
{
   if (allocation != VK_NULL_HANDLE) vmaUnmapMemory(allocator_, allocation);
}