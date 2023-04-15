#include "Fence.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"

using namespace rw::lif::vlk;

Fence::Fence(RedWolfManager& manager, const DeviceBase& device) : BaseObject(manager), device_{ device }
{
   VkFenceCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
   createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Create the fence as signalled so that the first wait completes successfully.

   fence_ = device_.createFence(createInfo);
   if (fence_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create fence.");
   }
}

Fence::Fence(Fence&& other) noexcept : BaseObject(other.manager_), fence_{ other.fence_ }, device_{ other.device_ }
{
   other.fence_ = VK_NULL_HANDLE;
}

Fence::~Fence()
{
   device_.destroyFence(fence_);
}

VkFence Fence::handle() const
{
   return fence_;
}

void Fence::reset()
{
   VkResult errCode{ vkResetFences(device_.handle(), 1U, &fence_) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to reset fence: {}", errCode);
   }
}

void Fence::wait()
{
   VkResult errCode{ vkWaitForFences(device_.handle(), 1, &fence_, VK_TRUE, std::chrono::nanoseconds::max().count()) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to wait on fence: {}", errCode);
   }
}

void Fence::waitAndReset()
{
   wait();
   reset();
}