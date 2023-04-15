#ifndef RW_LIF_VULKAN_VULKANPHYSICALDEVICE_HPP
#define RW_LIF_VULKAN_VULKANPHYSICALDEVICE_HPP

#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"
#include "RedWolf/lif/vulkan/custom_formatters.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
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
       * @brief Create a logical device.
       * @param deviceInfo Settings for the new device.
       * @return Handle to the new logical device or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkDevice createDevice(const VkDeviceCreateInfo& deviceInfo) const;

      /**
       * @brief Create a graphics device from this physical device.
       * @return Owning pointer to the new graphics device.
       */
      [[nodiscard]] std::unique_ptr<GraphicsDevice> createGraphicsDevice() const;

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

      /**
       * @brief Get the properties of a physical device.
       * @return Properties of the physical device.
       */
      [[nodiscard]] const VkPhysicalDeviceProperties& properties() const;

   private:
      mutable std::mutex mtx_; /**< Device mutex. */

      VkPhysicalDevice                 device_{ VK_NULL_HANDLE }; /**< Handle to the raw device. */
      VkPhysicalDeviceProperties       properties_;               /**< Device properties. */
      VkPhysicalDeviceFeatures         features_;                 /**< Device features. */
      VkPhysicalDeviceMemoryProperties memoryProperties_;         /**< Device memory properties. */

      std::vector<VkQueueFamilyProperties> queueFamilyProperties_; /**< Properties of all queue families supported by this device. */
      QueueFamilies                        queueFamilies_;         /**< Indices of the device's queue families. */
   };
} // namespace rw::lif::vlk

#endif