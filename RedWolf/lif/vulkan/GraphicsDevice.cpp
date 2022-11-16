#include "GraphicsDevice.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

using namespace rw::lif::vlk;

GraphicsDevice::GraphicsDevice(RedWolfManager& manager, PhysicalDevice& physicalDevice) :
   DeviceBase(manager, physicalDevice, required_queues, required_extensions)
{
}

GraphicsDevice::GraphicsDevice(GraphicsDevice&& other) noexcept :
   DeviceBase(other.manager_, other.physicalDevice_, required_queues, required_extensions)
{
}

VkDevice GraphicsDevice::handle()
{
   return device_;
}