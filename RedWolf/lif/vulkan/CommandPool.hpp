#ifndef RW_LIF_VULKAN_COMMANDPOOL_HPP
#define RW_LIF_VULKAN_COMMANDPOOL_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

namespace rw::lif::vlk
{
   class DeviceBase;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkCommandPool.
    */
   class RW_API CommandPool : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Logical device the commands will be delivered to.
       * @param queueType Type of queue this commands will be delivered to.
       */
      CommandPool(RedWolfManager& manager, const DeviceBase& device, QueueFamilies::Id queueType);

      /**
       * @brief Destructor.
       */
      ~CommandPool() override;

      /**
       * @brief Copy constructor.
       */
      CommandPool(const CommandPool&) = delete;

      /**
       * @brief Move constructor.
       */
      CommandPool(CommandPool&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      CommandPool& operator=(const CommandPool&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      CommandPool& operator=(CommandPool&&) = delete;

      /**
       * @brief Create a new command buffer.
       * @param commandBufferInfo Command buffer creation parameters.
       * @return Newly created command buffer, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkCommandBuffer createCommandBuffer(const VkCommandBufferAllocateInfo& commandBufferInfo) const;

      /**
       * @brief Get the raw handle of the command pool.
       * @return Raw handle of the command pool.
       */
      [[nodiscard]] VkCommandPool handle() const;

   private:
      VkCommandPool     commandPool_{ VK_NULL_HANDLE }; /**< Raw handle to the command pool. */
      const DeviceBase& device_;                        /**< Logical device this command pool is bound to. */
   };
} // namespace rw::lif::vlk

#endif