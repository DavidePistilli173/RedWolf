#include "VulkanManager.hpp"

#include "RedWolf/common.hpp"
#include "RedWolf/libif/GlfwManager.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

// VUlkan function pointers.
PFN_vkCreateDebugUtilsMessengerEXT  fptr_vkCreateDebugUtilsMessengerEXT{ nullptr };
PFN_vkDestroyDebugUtilsMessengerEXT fptr_vkDestroyDebugUtilsMessengerEXT{ nullptr };

std::mutex debugCallbackMutex; /**< Mutex for protecting the debug callback. */

/**
 * @brief Callback for Vulkan debug validation layers.
 */
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
   VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
   VkDebugUtilsMessageTypeFlagsEXT             messageType,
   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   void*                                       pUserData)
{
   std::scoped_lock lck{ debugCallbackMutex };

   rw::utils::Logger* logger{ reinterpret_cast<rw::utils::Logger*>(pUserData) };

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

/**
 * @brief Initialise a single Vulkan function.
 * @tparam FunctionT Function pointer type.
 * @param instance Vulkan instance.
 * @param function Variable where to store the function pointer.
 * @param functionName Name of the function to initialise.
 * @param logger Logger instance.
 */
template<typename FunctionT>
void initialiseFunction(VkInstance instance, FunctionT& function, std::string_view functionName, rw::utils::Logger* logger)
{
   function = reinterpret_cast<FunctionT>(vkGetInstanceProcAddr(instance, functionName.data()));
   if (function == nullptr)
   {
      logger->relFatal("Failed to initialise Vulkan function {}.", functionName);
   }
}

namespace rw::libif
{
   std::mutex               VulkanManager::mtx_;
   size_t                   VulkanManager::activeUsers_{ 0U };
   VkInstance               VulkanManager::instance_{ nullptr };
   VkDebugUtilsMessengerEXT VulkanManager::debugMessenger_;

   VulkanManager::VulkanManager(std::string_view appName, int verMajor, int verMinor, int verPatch) :
      logger_{ rw::utils::Logger::instance() }
   {
      std::scoped_lock lck{ mtx_ };
      if (activeUsers_ == 0U)
      {
         createInstance_(appName, verMajor, verMinor, verPatch);
         logger_->relInfo("Vulkan initialised.");
      }
      ++activeUsers_;
   }

   VulkanManager::~VulkanManager()
   {
      std::scoped_lock lck{ mtx_ };
      --activeUsers_;
      if (activeUsers_ == 0U)
      {
         if constexpr (rw::debug)
         {
            std::scoped_lock debugLck{ debugCallbackMutex };
            fptr_vkDestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
         }

         vkDestroyInstance(instance_, nullptr);
         logger_->relInfo("Vulkan cleaned-up.");
      }
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
      GlfwManager glfwManager;
      auto        requiredExtensions{ glfwManager.getRequiredVulkanInstanceExtensions() };
      if (requiredExtensions.empty())
      {
         logger_->relFatal("Vulkan not supported on the current platform.");
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
         debugInfo.pfnUserCallback = debugCallback;
         debugInfo.pUserData = rw::utils::Logger::instance();

         instanceInfo.pNext = &debugInfo;
      }

      // Create the instance.
      if (vkCreateInstance(&instanceInfo, nullptr, &instance_) != VK_SUCCESS)
      {
         logger_->fatal("Failed to create Vulkan instance.");
      }

      // Initialise Vulkan function pointers.
      initialiseFunctions_();

      // Set up debug messages
      if constexpr (rw::debug)
      {
         if (VkResult result{ fptr_vkCreateDebugUtilsMessengerEXT(instance_, &debugInfo, nullptr, &debugMessenger_) }; result != VK_SUCCESS)
         {
            logger_->err("Failed to initialise Vulkan debug messenger, error code {}.", static_cast<int>(result));
         }
      }
   }

   void VulkanManager::initialiseFunctions_()
   {
      initialiseFunction(instance_, fptr_vkCreateDebugUtilsMessengerEXT, "vkCreateDebugUtilsMessengerEXT", logger_);
      initialiseFunction(instance_, fptr_vkDestroyDebugUtilsMessengerEXT, "vkDestroyDebugUtilsMessengerEXT", logger_);
   }
} // namespace rw::libif