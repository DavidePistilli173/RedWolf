#include "Instance.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

Instance::Instance(
   RedWolfManager&                 manager,
   std::string_view                appName,
   const rw::dat::VersionInfo&     appVersion,
   const std::vector<const char*>& additionalExtensions) :
   InstanceBase(manager, appName, appVersion, additionalExtensions)
{
   init_();
}

Instance::~Instance()
{
   if constexpr (rw::debug)
   {
      functions_.vkDestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
   }
}

Instance::Instance(Instance&& other) noexcept :
   InstanceBase(other.manager_, other.appName_, other.appVersion_, other.additionalExtensions_), debugMessenger_{ other.debugMessenger_ },
   physicalDevices_{ std::move(other.physicalDevices_) }
{
   other.debugMessenger_ = VK_NULL_HANDLE;
}

void Instance::destroySurface(VkSurfaceKHR surface)
{
   if (surface == VK_NULL_HANDLE) return;
   vkDestroySurfaceKHR(instance_, surface, nullptr);
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
      VkResult errCode{ functions_.vkCreateDebugUtilsMessengerEXT(instance_, &debugInfo_, nullptr, &debugMessenger_) };
      if (errCode != VK_SUCCESS)
      {
         logger_.relErr("Failed to create Vulkan debug messenger.");
      }
   }

   // Initialise physical devices.
   std::vector<VkPhysicalDevice> availableDevices;
   u32                           deviceCount{ 0U };
   VkResult                      errCode{ vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr) };
   if (errCode == VK_SUCCESS && deviceCount > 0U)
   {
      availableDevices.resize(deviceCount);
      errCode = vkEnumeratePhysicalDevices(instance_, &deviceCount, availableDevices.data());
      if (errCode != VK_SUCCESS)
      {
         availableDevices.resize(0U);
      }
   }

   physicalDevices_.reserve(availableDevices.size());
   for (const auto& device : availableDevices)
   {
      physicalDevices_.emplace_back(std::make_unique<PhysicalDevice>(manager_, device));
   }
}