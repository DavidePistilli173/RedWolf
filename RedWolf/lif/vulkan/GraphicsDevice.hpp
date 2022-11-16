#ifndef RW_LIF_VULKAN_VULKANDEVICE_HPP
#define RW_LIF_VULKAN_VULKANDEVICE_HPP

#include "RedWolf/lif/vulkan/DeviceBase.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

#include <vector>
#include <vulkan/vulkan.h>

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
   class PhysicalDevice;
   class Surface;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan logical device that supports the graphics pipeline.
    */
   class RW_API GraphicsDevice : public DeviceBase
   {
   public:
      /**
       * @brief Ids of the queue families required for a graphics device.
       */
      static constexpr std::array required_queues{ QueueFamilies::Id::graphics, QueueFamilies::Id::presentation };

      /**
       * @brief List of extensions required for a graphics device.
       */
      static constexpr std::array required_extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param physicalDevice The physical device that this logical device will reference.
       */
      GraphicsDevice(RedWolfManager& manager, PhysicalDevice& physicalDevice);

      /**
       * @brief Destructor.
       */
      ~GraphicsDevice() override = default;

      /**
       * @brief Copy constructor.
       */
      GraphicsDevice(const GraphicsDevice&) = delete;

      /**
       * @brief Move constructor.
       * @param other Device to move from.
       */
      GraphicsDevice(GraphicsDevice&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      GraphicsDevice& operator=(const GraphicsDevice&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      GraphicsDevice& operator=(GraphicsDevice&&) = delete;

      /**
       * @brief Get the raw handle of the device.
       * @return Raw handle of the device.
       */
      [[nodiscard]] VkDevice handle();

   private:
   };
} // namespace rw::lif::vlk

#endif