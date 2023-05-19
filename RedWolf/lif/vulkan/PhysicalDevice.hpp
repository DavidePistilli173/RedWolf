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

   private:
      rw::util::Logger& logger_; /**< Logger. */

      VkPhysicalDevice                     physicalDevice_{ VK_NULL_HANDLE }; /**< Raw physical device handle. */
      VkPhysicalDeviceProperties           properties_;                       /**< Physical device properties. */
      VkPhysicalDeviceFeatures             features_;                         /**< Physical device features. */
      VkPhysicalDeviceMemoryProperties     memoryProperties_;                 /**< Physical device memory properties. */
      std::vector<VkQueueFamilyProperties> queueFamilyProperties_; /**< Properties of all queue families supported by this device. */
   };
} // namespace rw::lif::vulkan

#endif