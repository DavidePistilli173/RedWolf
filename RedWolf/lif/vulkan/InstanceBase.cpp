#include "InstanceBase.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DebugUtilsMessenger.hpp"

using namespace rw::lif::vlk;

InstanceBase::InstanceBase(
   RedWolfManager&                 manager,
   std::string_view                appName,
   const rw::dat::VersionInfo&     appVersion,
   const std::vector<const char*>& additionalExtensions,
   void*                           debugUserData) :
   manager_{ manager },
   logger_{ manager.logger() }, vulkanInterface_{ manager.vulkanInterface() }, appName_{ appName }, appVersion_{ appVersion },
   additionalExtensions_{ additionalExtensions }
{
   createInstance_(debugUserData);
   logger_.info("Vulkan instance created.");
}

InstanceBase::~InstanceBase()
{
   vulkanInterface_.destroyInstance(instance_);
}

VkInstance InstanceBase::handle()
{
   return instance_;
}

void InstanceBase::createInstance_(void* debugUserData)
{
   // Application information.
   VkApplicationInfo appInfo{};
   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appInfo.pApplicationName = appName_.data();
   appInfo.applicationVersion = VK_MAKE_VERSION(appVersion_.major, appVersion_.minor, appVersion_.patch);
   appInfo.pEngineName = "RedWolf";
   appInfo.engineVersion = VK_MAKE_VERSION(rw::version_major, rw::version_minor, rw::version_patch);
   appInfo.apiVersion = VK_API_VERSION_1_3;
   appInfo.pNext = nullptr;

   // Instance information.
   VkInstanceCreateInfo instanceInfo;
   instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   instanceInfo.pApplicationInfo = &appInfo;

   // Validation layers, enabled only for debug builds.
   if constexpr (rw::debug)
   {
      static std::vector<const char*> requiredValidationLayers{ "VK_LAYER_KHRONOS_validation" };

      instanceInfo.enabledLayerCount = requiredValidationLayers.size();
      instanceInfo.ppEnabledExtensionNames = requiredValidationLayers.data();

      // Debug extension.
      additionalExtensions_.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   }
   else
   {
      instanceInfo.enabledLayerCount = 0U;
   }

   // Swap chain extension.
   additionalExtensions_.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   // Set enabled extensions
   instanceInfo.enabledExtensionCount = additionalExtensions_.size();
   instanceInfo.ppEnabledExtensionNames = additionalExtensions_.data();

   // Enable debug messages for create/destroy instance functions.
   if constexpr (rw::debug)
   {
      debugInfo_.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debugInfo_.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debugInfo_.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debugInfo_.pfnUserCallback = DebugUtilsMessenger::debugCallback;
      debugInfo_.pUserData = debugUserData;

      instanceInfo.pNext = &debugInfo_;
   }

   // Create the instance.
   instance_ = vulkanInterface_.createInstance(instanceInfo);
   if (instance_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create Vulkan instance.");
   }
}