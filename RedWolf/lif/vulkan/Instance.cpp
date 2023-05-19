#include "Instance.hpp"

using namespace rw::lif::vulkan;

Instance::Instance(
   rw::util::Logger&               logger,
   std::string_view                appName,
   const rw::dat::VersionInfo&     appVersion,
   const std::vector<const char*>& additionalExtensions) :
   logger_{ logger }
{
   std::vector<const char*> extensions = additionalExtensions;

   // Application information.
   VkApplicationInfo appInfo{};
   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appInfo.pApplicationName = appName.data();
   appInfo.applicationVersion = VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
   appInfo.pEngineName = "RedWolf";
   appInfo.engineVersion = VK_MAKE_VERSION(rw::version_major, rw::version_minor, rw::version_patch);
   appInfo.apiVersion = VK_API_VERSION_1_2;
   appInfo.pNext = nullptr;

   // Instance information.
   VkInstanceCreateInfo instanceInfo{};
   instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   instanceInfo.pApplicationInfo = &appInfo;

   // Validation layers, enabled only for debug builds.
   if constexpr (rw::debug)
   {
      static std::vector<const char*> requiredValidationLayers{ "VK_LAYER_KHRONOS_validation" };

      instanceInfo.enabledLayerCount = static_cast<u32>(requiredValidationLayers.size());
      instanceInfo.ppEnabledLayerNames = requiredValidationLayers.data();

      // Debug extension.
      extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   }
   else
   {
      instanceInfo.enabledLayerCount = 0U;
   }

   // Set enabled extensions
   instanceInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
   instanceInfo.ppEnabledExtensionNames = extensions.data();

   // Enable debug messages for create/destroy instance functions.
   if constexpr (rw::debug)
   {
      debugInfo_.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debugInfo_.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debugInfo_.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debugInfo_.pfnUserCallback = debugCallback;
      debugInfo_.pUserData = &logger_;

      instanceInfo.pNext = &debugInfo_;
   }

   // Create the instance.
   VkResult errCode{ vkCreateInstance(&instanceInfo, nullptr, &instance_) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relFatal("Failed to create Vulkan instance: {}.", errCode);
   }

   if (!initialiseFunctions_())
   {
      vkDestroyInstance(instance_, nullptr);
      logger_.relFatal("Failed to initialise function pointers.");
   }

   logger_.info("Vulkan instance created.");

   // Set up debug messages
   if constexpr (rw::debug)
   {
      errCode = functions_.vkCreateDebugUtilsMessengerEXT(instance_, &debugInfo_, nullptr, &debugMessenger_);
      if (errCode != VK_SUCCESS)
      {
         logger_.relErr("Failed to create Vulkan debug messenger.");
      }
   }

   // Initialise physical devices.
   std::vector<VkPhysicalDevice> availablePhysicalDevices;
   u32                           physicalDeviceCount{ 0U };
   errCode = vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount, nullptr);
   if (errCode == VK_SUCCESS && physicalDeviceCount > 0U)
   {
      availablePhysicalDevices.resize(physicalDeviceCount);
      errCode = vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount, availablePhysicalDevices.data());
      if (errCode != VK_SUCCESS)
      {
         availablePhysicalDevices.resize(0U);
      }
   }

   physicalDevices_.reserve(availablePhysicalDevices.size());
   for (const auto& physicalDevice : availablePhysicalDevices)
   {
      physicalDevices_.emplace_back(logger_, physicalDevice);
   }
}

Instance::~Instance()
{
   if constexpr (rw::debug)
   {
      if (debugMessenger_ != VK_NULL_HANDLE) functions_.vkDestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
   }

   if (instance_ != VK_NULL_HANDLE) vkDestroyInstance(instance_, nullptr);
}

Instance::Instance(Instance&& other) noexcept :
   logger_{ other.logger_ }, instance_{ other.instance_ }, debugInfo_{ other.debugInfo_ }, debugMessenger_{ other.debugMessenger_ }
{
   other.debugMessenger_ = VK_NULL_HANDLE;
   other.instance_ = VK_NULL_HANDLE;
}

VkInstance Instance::handle() const
{
   return instance_;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(
   VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
   VkDebugUtilsMessageTypeFlagsEXT             messageType,
   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   void*                                       pUserData)
{
   auto* logger{ reinterpret_cast<rw::util::Logger*>(pUserData) };

   std::string type;
   switch (messageType)
   {
   case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      type = "GENERAL";
      break;
   case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      type = "SPECIFICATION";
      break;
   case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      type = "PERFORMANCE";
      break;
   default:
      type = "UNDEFINED";
      break;
   }

   switch (messageSeverity)
   {
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      logger->trace("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      logger->info("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      logger->warn("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      logger->err("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
      logger->fatal("|{}| {}", type, pCallbackData->pMessage);
      break;
   }

   return VK_FALSE;
}

bool Instance::initialiseFunctions_()
{
   bool result{ true };
   if constexpr (rw::debug)
   {
      result = result && getFunctionPointer_(functions_.vkCreateDebugUtilsMessengerEXT, "vkCreateDebugUtilsMessengerEXT");
      result = result && getFunctionPointer_(functions_.vkDestroyDebugUtilsMessengerEXT, "vkDestroyDebugUtilsMessengerEXT");
   }
   return result;
}