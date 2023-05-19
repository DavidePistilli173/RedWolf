#ifndef RW_LIF_VULKAN_PHYSICALDEVICE_HPP
#define RW_LIF_VULKAN_PHYSICALDEVICE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/util/Logger.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace rw::lif::vulkan
{
   /**
    * @brief Wrapper for a VkPhysicalDevice.
    */
   class RW_API PhysicalDevice
   {
   public:
      /**
       * @brief Constructor.
       * @param logger Logger.
       * @param physicalDevice Physical device handle to wrap.
       */
      PhysicalDevice(rw::util::Logger& logger, VkPhysicalDevice physicalDevice);

      /**
       * @brief Get the properties of the extensions supported by the physical device.
       * @return Properties of the extensions supported by the physical device.
       */
      [[nodiscard]] const std::vector<VkExtensionProperties>& extensionProperties() const;

      /**
       * @brief Get the features of the physical device.
       * @return Features of the physical device.
       */
      [[nodiscard]] const VkPhysicalDeviceFeatures& features() const;

      /**
       * @brief Get the device handle.
       * @return Handle of the physical device.
       */
      [[nodiscard]] VkPhysicalDevice handle() const;

      /**
       * @brief Get the memory properties of the device.
       */
      [[nodiscard]] const VkPhysicalDeviceMemoryProperties& memoryProperties() const;

      /**
       * @brief Get the properties of a physical device.
       * @return Properties of the physical device.
       */
      [[nodiscard]] const VkPhysicalDeviceProperties& properties() const;

      /**
       * @brief Get the properties of all supported queue families.
       * @return Properties of all supported queue families.
       */
      [[nodiscard]] const std::vector<VkQueueFamilyProperties> queueFamilyProperties() const;

   private:
      rw::util::Logger& logger_; /**< Logger. */

      VkPhysicalDevice                     physicalDevice_{ VK_NULL_HANDLE }; /**< Raw physical device handle. */
      VkPhysicalDeviceProperties           properties_;                       /**< Physical device properties. */
      VkPhysicalDeviceFeatures             features_;                         /**< Physical device features. */
      VkPhysicalDeviceMemoryProperties     memoryProperties_;                 /**< Physical device memory properties. */
      std::vector<VkExtensionProperties>   extensionProperties_;              /**< Physical device extension properties. */
      std::vector<VkQueueFamilyProperties> queueFamilyProperties_; /**< Properties of all queue families supported by this device. */
   };
} // namespace rw::lif::vulkan

#endif