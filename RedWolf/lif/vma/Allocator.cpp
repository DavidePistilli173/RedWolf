#include "Allocator.hpp"

#include "RedWolf/lif/vulkan/custom_formatters.hpp"

using namespace rw::lif::vma;

Allocator::Allocator(VkInstance vulkanInstance, VkPhysicalDevice physicalDevice, VkDevice device, rw::util::Logger& logger) :
   logger_{ logger }, instance_{ vulkanInstance }, physicalDevice_{ physicalDevice }, device_{ device }
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
