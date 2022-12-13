#include "DeviceBase.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"

using namespace rw::lif::vlk;

DeviceBase::~DeviceBase()
{
   vulkanInterface_.destroyDevice(device_);
}

DeviceBase::DeviceBase(DeviceBase&& other) noexcept :
   BaseObject(other.manager_), physicalDevice_{ other.physicalDevice_ }, device_{ other.device_ }, queues_{ other.queues_ }
{
   other.device_ = VK_NULL_HANDLE;
}

VkDevice DeviceBase::handle() const
{
   return device_;
}
