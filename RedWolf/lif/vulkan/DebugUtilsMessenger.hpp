#ifndef RW_LIF_VULKAN_DEBUGUTILSMESSENGER_HPP
#define RW_LIF_VULKAN_DEBUGUTILSMESSENGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"

#include <mutex>

namespace rw
{
   class RedWolfManager;
}

namespace rw::utils
{
   class Logger;
}

namespace rw::lif::vlk
{
   class RW_API DebugUtilsMessenger : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      DebugUtilsMessenger(RedWolfManager& manager);

      /**
       * @brief Destructor.
       */
      virtual ~DebugUtilsMessenger();

      /**
       * @brief Copy constructor.
       */
      DebugUtilsMessenger(const DebugUtilsMessenger&) = delete;

      /**
       * @brief Move constructor.
       */
      DebugUtilsMessenger(DebugUtilsMessenger&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      DebugUtilsMessenger& operator=(const DebugUtilsMessenger&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      DebugUtilsMessenger& operator=(DebugUtilsMessenger&&) = delete;

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
       * @brief Initialise the debug messenger.
       * @param args Initialisation arguments.
       * @return true on success, false otherwise.
       */
      bool initialise(const VkDebugUtilsMessengerCreateInfoEXT& args);

   private:
      std::mutex               debugCallbackMutex_; /**< Mutex for protecting the debug callback. */
      VkDebugUtilsMessengerEXT messenger_;          /**< Raw messenger handle. */
   };
} // namespace rw::lif::vlk

#endif