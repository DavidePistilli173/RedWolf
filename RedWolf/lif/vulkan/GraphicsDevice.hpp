#ifndef RW_LIF_VULKAN_VULKANDEVICE_HPP
#define RW_LIF_VULKAN_VULKANDEVICE_HPP

#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

#include <vector>
#include <vulkan/vulkan.h>

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
   class PhysicalDevice;
   class Surface;

   /**
    * @brief Wrapper for a Vulkan logical device that supports the graphics pipeline.
    */
   class RW_API GraphicsDevice
   {
   public:
      friend class PhysicalDevice;

   private:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param physicalDevice The physical device that this logical device will reference.
       * @param surface Surface that will be used with this device.
       */
      GraphicsDevice(RedWolfManager& manager, PhysicalDevice& physicalDevice, Surface& surface);

      /**
       * @brief Destructor.
       */
      ~GraphicsDevice();

      /**
       * @brief Copy constructor.
       */
      GraphicsDevice(const GraphicsDevice&) = delete;

      /**
       * @brief Move constructor.
       */
      GraphicsDevice(GraphicsDevice& other);

      /**
       * @brief Copy-assignment operator.
       */
      GraphicsDevice& operator=(const GraphicsDevice&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      GraphicsDevice& operator=(GraphicsDevice&& other);

      /**
       * @brief Initialise the queue data for device creation.
       * @param availableQueueFamilies List of available queues.
       */
      [[nodiscard]] std::vector<VkDeviceQueueCreateInfo> initialiseQueueData_(const QueueFamilies& availableQueueFamilies);

      rw::utils::Logger& logger_;          /**< Library logger. */
      Interface&         vulkanInterface_; /**< Vulkan library manager. */

      VkPhysicalDevice physicalDevice_; /**< Physical device that owns this logical device. */

      VkDevice device_{ VK_NULL_HANDLE };            /**< Raw handle to the logical device. */
      VkQueue  graphicsQueue_{ VK_NULL_HANDLE };     /**< Handle to the graphics queue. */
      VkQueue  presentationQueue_{ VK_NULL_HANDLE }; /**< Handle to the image presentation queue. */
   };
} // namespace rw::lif::vlk

#endif