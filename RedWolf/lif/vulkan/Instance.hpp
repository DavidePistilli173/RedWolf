#ifndef RW_LIF_VULKAN_INSTANCE_HPP
#define RW_LIF_VULKAN_INSTANCE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/VersionInfo.hpp"
#include "RedWolf/lif/vulkan/FunctionPointers.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/custom_formatters.hpp"
#include "RedWolf/util/Logger.hpp"

#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

namespace rw::lif::vulkan
{
   class RW_API Instance
   {
   public:
      /**
       * @brief Constructor.
       * @param logger Logger.
       * @param appName Name of the application.
       * @param appVersion Version number of the application.
       * @param additionalExtensions Additional Vulkan extensions that have to be enabled.
       */
      Instance(
         rw::util::Logger&               logger,
         std::string_view                appName,
         const rw::dat::VersionInfo&     appVersion,
         const std::vector<const char*>& additionalExtensions);

      /**
       * @brief Destructor.
       */
      virtual ~Instance();

      /**
       * @brief Copy constructor.
       */
      Instance(const Instance&) = delete;

      /**
       * @brief Move constructor.
       * @param other Instance to move from.
       */
      Instance(Instance&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      Instance& operator=(const Instance&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Instance& operator=(Instance&&) = delete;

      /**
       * @brief Get the raw handle to the instance.
       * @return Raw handle to the instance.
       */
      [[nodiscard]] VkInstance handle() const;

   private:
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

      rw::util::Logger& logger_; /**< Logger. */

      VkInstance       instance_{ VK_NULL_HANDLE }; /**< Raw instance handle. */
      FunctionPointers functions_;                  /**< Instance-specific functions. */

      VkDebugUtilsMessengerCreateInfoEXT debugInfo_{};                      /**< Settings for the debug messenger. */
      VkDebugUtilsMessengerEXT           debugMessenger_{ VK_NULL_HANDLE }; /**< Vulkan debug messenger. */

      std::vector<PhysicalDevice> physicalDevices_; /**< List of supported physical devices. */
   };
} // namespace rw::lif::vulkan

#endif