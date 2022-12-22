#ifndef RW_LIF_VULKAN_COMMANDBUFFER_HPP
#define RW_LIF_VULKAN_COMMANDBUFFER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/CommandPool.hpp"
#include "RedWolf/lif/vulkan/FrameBuffer.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class GraphicsDevice;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkCommandBuffer.
    */
   class RW_API CommandBuffer : public BaseObject
   {
   public:
      static constexpr VkClearValue default_clear_colour{ { { 0.0F, 0.0F, 0.0F, 1.0F } } }; /**< Default clear colour. */

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param graphicsDevice Graphics device that will execute this command buffer.
       * @param commandPool Command pool this buffer will be allocated on.
       */
      CommandBuffer(RedWolfManager& manager, const GraphicsDevice& graphicsDevice, const CommandPool& commandPool);

      /**
       * @brief Destructor.
       */
      ~CommandBuffer() override = default;

      /**
       * @brief Copy constructor.
       */
      CommandBuffer(const CommandBuffer&) = delete;

      /**
       * @brief Move constructor.
       */
      CommandBuffer(CommandBuffer&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      CommandBuffer& operator=(const CommandBuffer&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      CommandBuffer& operator=(CommandBuffer&&) = delete;

      /**
       * @brief Get the raw handle tot the command buffer.
       * @return Raw handle to the command buffer.
       */
      [[nodiscard]] VkCommandBuffer handle() const;

      /**
       * @brief Start recording this command buffer.
       */
      void beginRecording();

      /**
       * @brief Start a render pass.
       * @param renderPass Render pass to begin.
       * @param frameBuffer Frame buffer to render on.
       */
      void beginRenderPass(const RenderPass& renderPass, const FrameBuffer& frameBuffer);

      /**
       * @brief Stop recording this command buffer.
       */
      void endRecording();

   private:
      VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE }; /**< Raw handle to the command buffer. */

      bool recording_{ false }; /**< If true, the command buffer is currently being recorded. */
   };
} // namespace rw::lif::vlk

#endif