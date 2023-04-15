#ifndef RW_LIF_VULKAN_INSTANCEBASE_HPP
#define RW_LIF_VULKAN_INSTANCEBASE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/VersionInfo.hpp"
#include "RedWolf/lif/vulkan/FunctionPointers.hpp"

#include <string>
#include <vector>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::utils
{
   class Logger;
} // namespace rw::utils

namespace rw::lif::vlk
{
   /**
    * @brief Container for a raw VkInstance handler.
    * @details This class is necessary to ensure that the instance is destroyed after all objects that use it.
    */
   class RW_API InstanceBase
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param appName Name of the application.
       * @param appVersion Version number of the application.
       * @param additionalExtensions Additional Vulkan extensions that have to be enabled.
       */
      InstanceBase(
         RedWolfManager&                 manager,
         std::string_view                appName,
         const rw::dat::VersionInfo&     appVersion,
         const std::vector<const char*>& additionalExtensions);

      /**
       * @brief Destructor.
       */
      virtual ~InstanceBase();

      /**
       * @brief Copy constructor.
       */
      InstanceBase(const InstanceBase&) = delete;

      /**
       * @brief Move constructor.
       * @param other Instance to move from.
       */
      InstanceBase(InstanceBase&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      InstanceBase& operator=(const InstanceBase&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      InstanceBase& operator=(InstanceBase&&) = delete;

      /**
       * @brief Get the raw handle to the instance.
       * @return Raw handle to the instance.
       */
      [[nodiscard]] VkInstance handle() const;

   protected:
      RedWolfManager&    manager_; /**< RedWolf library manager. */
      rw::utils::Logger& logger_;  /**< Library logger. */

      VkInstance       instance_{ VK_NULL_HANDLE }; /**< Raw instance handle. */
      FunctionPointers functions_;                  /**< Additional function pointers that will be loaded at runtime. */

      VkDebugUtilsMessengerCreateInfoEXT debugInfo_{}; /**< Settings for the debug messenger. */

      std::string              appName_;              /**< Name of the application that uses the instance. */
      rw::dat::VersionInfo     appVersion_;           /**< Version of the application that uses the instance. */
      std::vector<const char*> additionalExtensions_; /**< Additional required extensions for the instance. */

   private:
      /**
       * @brief Create the Vulkan instance.
       */
      void createInstance_();

      /**
       * @brief Callback for Vulkan debug validation layers.
       * @param messageSeverity Severity of the debug message.
       * @param messageType Type of the debug message.
       * @param pCallbackData Actual message and related data.
       * @param pUserData Custom user data.
       */
      static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
         VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT             messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
         void*                                       pUserData);

      /**
       * @brief Initialise a single Vulkan function.
       * @tparam FunctionT Function pointer type.
       * @param address Address to store the pointer.
       * @param functionName Name of the function to initialise.
       * @return true on success, false otherwise.
       */
      template<typename FunctionT>
      [[nodiscard]] bool getFunctionPointer_(FunctionT& address, std::string_view functionName)
      {
         address = reinterpret_cast<FunctionT>(vkGetInstanceProcAddr(instance_, functionName.data()));
         return address != nullptr;
      }

      /**
       * @brief Initialise the function pointers.
       */
      [[nodiscard]] bool initialiseFunctions_();
   };
} // namespace rw::lif::vlk

#endif