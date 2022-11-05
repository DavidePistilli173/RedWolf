#include "Instance.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"

using namespace rw::lif::vlk;

Instance::Instance(
   RedWolfManager&                 manager,
   std::string_view                appName,
   const rw::dat::VersionInfo&     appVersion,
   const std::vector<const char*>& additionalExtensions) :
   InstanceBase(manager, appName, appVersion, additionalExtensions),
   debugMessenger_{ manager }
{
   init_();
}

PhysicalDevice* Instance::getNextPhysicalDevice(PhysicalDevice::OpCategory requiredCategory, PhysicalDevice* previousDevice)
{
   for (auto& device : physicalDevices_)
   {
      if (device.get() != previousDevice && device->isOpCategorySupported(requiredCategory))
      {
         return device.get();
      }
   }

   return nullptr;
}

void Instance::init_()
{
   // Set up debug messages
   if constexpr (rw::debug)
   {
      if (!debugMessenger_.initialise(debugInfo_))
      {
         logger_.err("Failed to initialise Vulkan debug messenger.");
      }
   }

   // Initialise physical devices.
   auto availableDevices{ vulkanInterface_.getPhysicalDevices(instance_) };
   physicalDevices_.reserve(availableDevices.size());
   for (const auto& device : availableDevices)
   {
      physicalDevices_.emplace_back(std::make_unique<PhysicalDevice>(manager_, device));
   }
}