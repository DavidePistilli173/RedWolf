#include "CommandBuffer.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

using namespace rw::lif::vlk;

CommandBuffer::CommandBuffer(RedWolfManager& manager, const GraphicsDevice& graphicsDevice, const CommandPool& commandPool) :
   BaseObject(manager)
{
   VkCommandBufferAllocateInfo allocateInfo{};
   allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
   allocateInfo.commandPool = commandPool.handle();
   allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
   allocateInfo.commandBufferCount = 1U;

   commandBuffer_ = vulkanInterface_.allocateCommandBuffer(graphicsDevice.handle(), allocateInfo);
   if (commandBuffer_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create command buffer.");
   }
}

VkCommandBuffer CommandBuffer::handle() const
{
   return commandBuffer_;
}

void CommandBuffer::beginRecording()
{
   if (!recording_)
   {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = 0;
      beginInfo.pInheritanceInfo = nullptr;

      if (!vulkanInterface_.beginCommandBuffer(commandBuffer_, beginInfo))
      {
         logger_.relFatal("Failed to start the Vulkan command buffer recording.");
      }

      recording_ = true;
   }
}

void CommandBuffer::beginRenderPass(const RenderPass& renderPass, const FrameBuffer& frameBuffer)
{
   if (recording_)
   {
      VkRenderPassBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      beginInfo.renderPass = renderPass.handle();
      beginInfo.framebuffer = frameBuffer.handle();
      beginInfo.renderArea.offset = { 0, 0 };
      beginInfo.renderArea.extent = frameBuffer.extent();
      beginInfo.clearValueCount = 1U;
      beginInfo.pClearValues = &default_clear_colour;

      vulkanInterface_.cmdBeginRenderPass(commandBuffer_, beginInfo);
      renderPassActive_ = true;
   }
}

/**
 * @brief Stop recording this command buffer.
 */
void CommandBuffer::endRecording()
{
   if (recording_)
   {
      if (!vulkanInterface_.endCommandBuffer(commandBuffer_))
      {
         logger_.relFatal("Failed to complete the Vulkan command buffer recording.");
      }

      recording_ = false;
   }
}

void CommandBuffer::endRenderPass()
{
   if (renderPassActive_)
   {
      vulkanInterface_.cmdEndRenderPass(commandBuffer_);
      renderPassActive_ = false;
   }
}