#include "VulkanManager.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/common.hpp"

#include <string>
#include <vector>

namespace rw::libif
{
   VulkanManager::VulkanManager(RedWolfManager& manager, std::string_view appName, int verMajor, int verMinor, int verPatch) :
      logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }
   {
      createInstance_(appName, verMajor, verMinor, verPatch);
      logger_.relInfo("Vulkan initialised.");
   }

   VulkanManager::~VulkanManager()
   {
      if constexpr (rw::debug)
      {
         std::scoped_lock debugLck{ debugCallbackMutex_ };
         vkDestroyDebugUtilsMessengerEXT_(vulkanInstance_, debugMessenger_, nullptr);
      }

      vkDestroyInstance(vulkanInstance_, nullptr);
      logger_.relInfo("Vulkan cleaned-up.");
   }

   void VulkanManager::createInstance_(std::string_view appName, int verMajor, int verMinor, int verPatch)
   {
      // Application information.
      VkApplicationInfo appInfo;
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = appName.data();
      appInfo.applicationVersion = VK_MAKE_VERSION(verMajor, verMinor, verPatch);
      appInfo.pEngineName = "RedWolf";
      appInfo.engineVersion = VK_MAKE_VERSION(rw::version_major, rw::version_minor, rw::version_patch);
      appInfo.apiVersion = VK_API_VERSION_1_3;
      appInfo.pNext = nullptr;

      // Instance information.
      VkInstanceCreateInfo instanceInfo;
      instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      instanceInfo.pApplicationInfo = &appInfo;

      // Required GLFW extensions.
      auto requiredExtensions{ glfwManager_.getRequiredVulkanInstanceExtensions() };
      if (requiredExtensions.empty())
      {
         logger_.relFatal("Vulkan not supported on the current platform.");
      }

      // Validation layers, enabled only for debug builds.
      if constexpr (rw::debug)
      {
         static std::vector<const char*> requiredValidationLayers{ "VK_LAYER_KHRONOS_validation" };

         instanceInfo.enabledLayerCount = requiredValidationLayers.size();
         instanceInfo.ppEnabledExtensionNames = requiredValidationLayers.data();

         // Debug extension.
         requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }
      else
      {
         instanceInfo.enabledLayerCount = 0U;
      }

      // Set enabled extensions
      instanceInfo.enabledExtensionCount = requiredExtensions.size();
      instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

      // Enable debug messages for create/destroy instance functions.
      VkDebugUtilsMessengerCreateInfoEXT debugInfo;
      if constexpr (rw::debug)
      {
         debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
         debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
         debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
         debugInfo.pfnUserCallback = debugCallback_;
         debugInfo.pUserData = &logger_;

         instanceInfo.pNext = &debugInfo;
      }

      // Create the instance.
      if (vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance_) != VK_SUCCESS)
      {
         logger_.fatal("Failed to create Vulkan instance.");
      }

      // Initialise Vulkan function pointers.
      initialiseFunctions_();

      // Set up debug messages
      if constexpr (rw::debug)
      {
         if (VkResult result{ vkCreateDebugUtilsMessengerEXT_(vulkanInstance_, &debugInfo, nullptr, &debugMessenger_) };
             result != VK_SUCCESS)
         {
            logger_.err("Failed to initialise Vulkan debug messenger, error code {}.", static_cast<int>(result));
         }
      }
   }

   VKAPI_ATTR VkBool32 VKAPI_CALL VulkanManager::debugCallback_(
      VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT             messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void*                                       pUserData)
   {
      auto thisPtr{ reinterpret_cast<VulkanManager*>(pUserData) };

      std::scoped_lock lck{ thisPtr->debugCallbackMutex_ };

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
         thisPtr->logger_.trace("|{}| {}", type, pCallbackData->pMessage);
         break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
         thisPtr->logger_.info("|{}| {}", type, pCallbackData->pMessage);
         break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
         thisPtr->logger_.warn("|{}| {}", type, pCallbackData->pMessage);
         break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
         thisPtr->logger_.err("|{}| {}", type, pCallbackData->pMessage);
         break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
         thisPtr->logger_.fatal("|{}| {}", type, pCallbackData->pMessage);
         break;
      }

      return VK_FALSE;
   }

   void VulkanManager::initialiseFunctions_()
   {
      initialiseFunction_(vulkanInstance_, vkCreateDebugUtilsMessengerEXT_, "vkCreateDebugUtilsMessengerEXT");
      initialiseFunction_(vulkanInstance_, vkDestroyDebugUtilsMessengerEXT_, "vkDestroyDebugUtilsMessengerEXT");
   }
} // namespace rw::libif