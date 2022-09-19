#ifndef RW_LIF_VULKAN_INSTANCE_HPP
#define RW_LIF_VULKAN_INSTANCE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/VersionInfo.hpp"
#include "RedWolf/lif/vulkan/DebugUtilsMessenger.hpp"
#include "RedWolf/lif/vulkan/InstanceBase.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"

#include <memory>
#include <string>
#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   /**
    * @brief Single Vulkan instance.
    */
   class RW_API Instance : public InstanceBase
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param appName Name of the application.
       * @param appVersion Version number of the application.
       * @param additionalExtensions Additional Vulkan extensions that have to be enabled.
       */
      Instance(
         RedWolfManager&                 manager,
         std::string_view                appName,
         const rw::dat::VersionInfo&     appVersion,
         const std::vector<const char*>& additionalExtensions);

      /**
       * @brief Destructor.
       */
      virtual ~Instance() = default;

      /**
       * @brief Copy constructor.
       */
      Instance(const Instance&) = delete;

      /**
       * @brief Move constructor.
       */
      Instance(Instance&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      Instance& operator=(const Instance&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Instance& operator=(Instance&& other) = delete;

      /**
       * @brief Get the first physical device that supports the required operations after the one specified.
       * @param requiredCategory Required operations that the device will have to perform.
       * @param previousDevice Last device to ignore in the search.
       * @return Appropriate physical device, if it exists.
       */
      [[nodiscard]] PhysicalDevice*
         getNextPhysicalDevice(PhysicalDevice::OpCategory requiredCategory, PhysicalDevice* previousDevice = nullptr);

   private:
      /**
       * @brief Initialise instance data.
       */
      void init_();

      DebugUtilsMessenger debugMessenger_; /**< Vulkan debug messenger. */

      std::vector<std::unique_ptr<PhysicalDevice>> physicalDevices_; /**< List of all available physical devices. */
   };
} // namespace rw::lif::vlk

#endif