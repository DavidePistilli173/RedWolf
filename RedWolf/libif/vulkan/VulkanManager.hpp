#ifndef RW_LIBIF_VULKANMANAGER_HPP
#define RW_LIBIF_VULKANMANAGER_HPP

#include "RedWolf/utils/Logger.hpp"

#include <memory>
#include <mutex>
#include <string_view>
#include <vulkan/vulkan.h>

namespace rw::libif
{
   /**
    * @brief Interface for the Vulkan library.
    */
   class RW_API VulkanManager
   {
   public:
      /**
       * @brief Get the instance of the vulkan manager.
       * @param appName Name of the application.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       * @return Instance of the vulkan manager.
       */
      static VulkanManager* instance(std::string_view appName, int verMajor, int verMinor, int verPatch);

   private:
      /**
       * @brief Constructor.
       * @param appName Name of the application.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       */
      VulkanManager(std::string_view appName, int verMajor, int verMinor, int verPatch);

      /**
       * @brief Destructor.
       */
      ~VulkanManager();

      /**
       * @brief Create the Vulkan instance.
       * @param appName Name of the application.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       */
      void createInstance_(std::string_view appName, int verMajor, int verMinor, int verPatch);

      /**
       * @brief Callback for Vulkan debug validation layers.
       * @param messageSeverity Severity of the debug message.
       * @param messageType Type of the debug message.
       * @param pCallbackData Actual message and related data.
       * @param pUserData Custom user data.
       */
      static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback_(
         VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT             messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
         void*                                       pUserData);

      /**
       * @brief Initialise a single Vulkan function.
       * @tparam FunctionT Function pointer type.
       * @param instance Vulkan instance.
       * @param function Variable where to store the function pointer.
       * @param functionName Name of the function to initialise.
       * @param logger Logger instance.
       */
      template<typename FunctionT>
      void initialiseFunction_(VkInstance instance, FunctionT& function, std::string_view functionName, rw::utils::Logger* logger)
      {
         function = reinterpret_cast<FunctionT>(vkGetInstanceProcAddr(instance, functionName.data()));
         if (function == nullptr)
         {
            logger->relFatal("Failed to initialise Vulkan function {}.", functionName);
         }
      }

      /**
       * @brief Initialise all Vulkan function pointers.
       */
      void initialiseFunctions_();

      static std::mutex                                               mtx_;      /**< Mutex for protecting access to the manager. */
      static std::unique_ptr<VulkanManager, void (*)(VulkanManager*)> instance_; /**< Instance of the manager. */

      rw::utils::Logger* logger_; /**< Logger instance. */

      VkInstance vulkanInstance_; /**< Vulkan instance. */

      std::mutex               debugCallbackMutex_; /**< Mutex for protecting the debug callback. */
      VkDebugUtilsMessengerEXT debugMessenger_;     /**< Vulkan debug information. */

      // Vulkan function pointers.
      PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT_{ nullptr };
      PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT_{ nullptr };
   };
} // namespace rw::libif

#endif