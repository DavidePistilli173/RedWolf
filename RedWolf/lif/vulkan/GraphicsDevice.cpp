#include "GraphicsDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

GraphicsDevice::GraphicsDevice(RedWolfManager& manager, PhysicalDevice& physicalDevice) :
   DeviceBase(manager, physicalDevice, required_queues)
{
}

GraphicsDevice::~GraphicsDevice()
{
   vulkanInterface_.destroyDevice(device_);
}

GraphicsDevice::GraphicsDevice(GraphicsDevice&& other) : DeviceBase(other.manager_, other.physicalDevice_, other.required_queues) {}

VkDevice GraphicsDevice::handle()
{
   return device_;
}