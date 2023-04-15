#ifndef RW_LIF_VULKAN_SEMAPHORE_HPP
#define RW_LIF_VULKAN_SEMAPHORE_HPP

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
    * @brief Wrapper for a Vulkan VkSemaphore.
    */
   class RW_API Semaphore : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Logical device this semaphore will be used on.
       */
      Semaphore(RedWolfManager& manager, const DeviceBase& device);

      /**
       * @brief Destructor.
       */
      ~Semaphore() override;

      /**
       * @brief Copy constructor.
       */
      Semaphore(const Semaphore&) = delete;

      /**
       * @brief Move constructor.
       * @param other Semaphore to move from.
       */
      Semaphore(Semaphore&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      Semaphore& operator=(const Semaphore&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Semaphore& operator=(Semaphore&&) = delete;

      /**
       * @brief Get the raw handle to the semaphore.
       * @return Raw handle to the semaphore.
       */
      [[nodiscard]] VkSemaphore handle() const;

   private:
      VkSemaphore semaphore_{ VK_NULL_HANDLE }; /**< Raw handle to the semaphore. */

      const DeviceBase& device_; /**< Logical device this semaphore will be used on. */
   };
} // namespace rw::lif::vlk

#endif