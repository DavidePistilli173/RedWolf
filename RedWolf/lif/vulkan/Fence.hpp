#ifndef RW_LIF_VULKAN_FENCE_HPP
#define RW_LIF_VULKAN_FENCE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class DeviceBase;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkFence.
    */
   class RW_API Fence : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Logical device this fence will be used on.
       */
      Fence(RedWolfManager& manager, const DeviceBase& device);

      /**
       * @brief Destructor.
       */
      ~Fence() override;

      /**
       * @brief Copy constructor.
       */
      Fence(const Fence&) = delete;

      /**
       * @brief Move constructor.
       * @param other Fence to move from.
       */
      Fence(Fence&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      Fence& operator=(const Fence&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Fence& operator=(Fence&&) = delete;

      /**
       * @brief Get the raw handle to the fence.
       * @return Raw handle to the fence.
       */
      [[nodiscard]] VkFence handle() const;

      /**
       * @brief Reset the fence so that it can be signalled again.
       */
      void reset();

      /**
       * @brief Wait for the fence to be signalled.
       * @detail Note that this is a blocking call.
       */
      void wait();

      /**
       * @brief Wait for the fence to be signalled and then reset it.
       * @detail Note that this is a blocking call.
       */
      void waitAndReset();

   private:
      VkFence fence_{ VK_NULL_HANDLE }; /**< Raw handle to the fence. */

      const DeviceBase& device_; /**< Logical device this fence will be used on. */
   };
} // namespace rw::lif::vlk

#endif