#ifndef RW_LIF_VULKAN_VULKANPHYSICALDEVICE_HPP
#define RW_LIF_VULKAN_VULKANPHYSICALDEVICE_HPP

#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   class GraphicsDevice;
} // namespace rw::lif::vlk

namespace rw::utils
{
   class Logger;
} // namespace rw::utils

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan physical device.
    */
   class RW_API PhysicalDevice : public BaseObject
   {
   public:
      /**
       * @brief Categories of operations a device can perform.
       */
      enum class OpCategory
      {
         graphics
      };

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      PhysicalDevice(RedWolfManager& manager, VkPhysicalDevice device);

      /**
       * @brief Get the available queue families for the current device.
       * @return Data on the available queue families for the current device.
       */
      [[nodiscard]] QueueFamilies availableQueueFamilies() const;

      /**
       * @brief Create a graphics device from this physical device.
       * @return Owning pointer to the new graphics device.
       */
      [[nodiscard]] std::unique_ptr<GraphicsDevice> createGraphicsDevice();

      /**
       * @brief Get the device handle.
       * @return Handle of the physical device.
       */
      [[nodiscard]] VkPhysicalDevice handle();

      /**
       * @brief Check whether the device supports operations from a specific category.
       * @param cat Category to check support for.
       * @return true if the devices supports the category, false otherwise.
       */
      [[nodiscard]] bool isOpCategorySupported(OpCategory cat) const;

      /**
       * @brief Check whether a specific surface is supported and update the queue family information if necessary.
       * @param surface Surface to check compatibility for.
       * @return true if the device can support the surface, false otherwise.
       */
      [[nodiscard]] bool isSurfaceSupported(VkSurfaceKHR surface);

   private:
      mutable std::mutex mtx_; /**< Device mutex. */

      VkPhysicalDevice           device_{ VK_NULL_HANDLE }; /**< Handle to the raw device. */
      VkPhysicalDeviceProperties properties_;               /**< Device properties. */
      VkPhysicalDeviceFeatures   features_;                 /**< Device features. */

      std::vector<VkQueueFamilyProperties> queueFamilyProperties_; /**< Properties of all queue families supported by this device. */
      QueueFamilies                        queueFamilies_;         /**< Indices of the device's queue families. */
   };
} // namespace rw::lif::vlk

#endif