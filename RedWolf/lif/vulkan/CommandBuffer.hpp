#ifndef RW_LIF_VULKAN_COMMANDBUFFER_HPP
#define RW_LIF_VULKAN_COMMANDBUFFER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/CommandPool.hpp"
#include "RedWolf/lif/vulkan/FrameBuffer.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class GraphicsDevice;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkCommandBuffer.
    * @details All instances created from a single command pool must be used from the same thread.
    */
   class RW_API CommandBuffer : public BaseObject
   {
   public:
      static constexpr VkClearValue default_clear_colour{ { { 0.2F, 0.0F, 0.0F, 1.0F } } }; /**< Default clear colour. */

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
       * @param other Command buffer to move from.
       */
      CommandBuffer(CommandBuffer&& other) noexcept;

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
      void beginRenderPass(VkRenderPass renderPass, const FrameBuffer& frameBuffer);

      /**
       * @brief Bind a pipeline to the command buffer.
       * @param pipeline Pipeline to bind.
       * @param pipelineType Type of pipeline to bind.
       */
      void bindPipeline(VkPipeline pipeline, VkPipelineBindPoint pipelineType);

      /**
       * @brief Bind a vertex buffer to the command buffer.
       * @param buffer Vertex buffer to bind.
       */
      void bindVertexBuffer(VkBuffer buffer);

      /**
       * @brief Copy data from a buffer to another.
       * @param dstBuffer Destination buffer.
       * @param srcBuffer Source buffer.
       * @param copyInfo Copy parameters.
       */
      void copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, const VkBufferCopy& copyInfo);

      /**
       * @brief Draw vertices.
       * @param vertexCount Number of vertices to draw.
       * @param instanceCount Number of instances of the same set of vertices to draw.
       * @param firstVertex Offset in the vertex buffer for the first vertex to draw.
       * @param firstInstance Offset in the set of instances for the first instance to draw.
       */
      void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

      /**
       * @brief Stop recording this command buffer.
       */
      void endRecording();

      /**
       * @brief End the ongoing render pass
       */
      void endRenderPass();

      /**
       * @brief Reset the command buffer.
       */
      void reset();

      /**
       * @brief Set the scissor for the command buffer.
       * @param scissor Scissor to set.
       */
      void setScissor(const VkRect2D& scissor);

      /**
       * @brief Set the viewport for the command buffer.
       * @param viewport Viewport to set.
       */
      void setViewport(const VkViewport& viewport);

   private:
      VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE }; /**< Raw handle to the command buffer. */

      bool recording_{ false };        /**< If true, the command buffer is currently being recorded. */
      bool renderPassActive_{ false }; /**< If true, there is an active render pass. */
   };
} // namespace rw::lif::vlk

#endif