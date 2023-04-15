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

   commandBuffer_ = commandPool.createCommandBuffer(allocateInfo);
   if (commandBuffer_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create command buffer.");
   }
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept :
   BaseObject(other.manager_), commandBuffer_{ other.commandBuffer_ }, recording_{ other.recording_ }, renderPassActive_{
      other.renderPassActive_
   }
{
   other.commandBuffer_ = VK_NULL_HANDLE;
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
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
      beginInfo.pInheritanceInfo = nullptr;

      VkResult errCode{ vkBeginCommandBuffer(commandBuffer_, &beginInfo) };
      if (errCode != VK_SUCCESS)
      {
         logger_.relFatal("Failed to start command buffer recording: {}", errCode);
      }

      recording_ = true;
   }
}

void CommandBuffer::beginRenderPass(VkRenderPass renderPass, const FrameBuffer& frameBuffer)
{
   if (renderPass == VK_NULL_HANDLE)
   {
      logger_.warn("Null render pass.");
      return;
   }

   if (recording_ && !renderPassActive_)
   {
      VkRenderPassBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      beginInfo.renderPass = renderPass;
      beginInfo.framebuffer = frameBuffer.handle();
      beginInfo.renderArea.offset = { 0, 0 };
      beginInfo.renderArea.extent = frameBuffer.extent();
      beginInfo.clearValueCount = 1U;
      beginInfo.pClearValues = &default_clear_colour;

      vkCmdBeginRenderPass(commandBuffer_, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
      renderPassActive_ = true;
   }
   else
   {
      logger_.warn("Command buffer in the wrong state: recording = {}, render pass active = {}", recording_, renderPassActive_);
   }
}

void CommandBuffer::bindPipeline(VkPipeline pipeline, VkPipelineBindPoint pipelineType)
{
   if (pipeline == VK_NULL_HANDLE)
   {
      logger_.warn("Null pipeline.");
      return;
   }

   if (recording_)
   {
      vkCmdBindPipeline(commandBuffer_, pipelineType, pipeline);
   }
   else
   {
      logger_.warn("Command buffer not recording.");
   }
}

void CommandBuffer::bindVertexBuffer(VkBuffer buffer)
{
   if (buffer == VK_NULL_HANDLE)
   {
      logger_.warn("Null buffer.");
      return;
   }

   if (recording_)
   {
      VkDeviceSize offsets[] = { 0U };
      vkCmdBindVertexBuffers(commandBuffer_, 0U, 1U, &buffer, offsets);
   }
   else
   {
      logger_.warn("Command buffer not recording.");
   }
}

void CommandBuffer::copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, const VkBufferCopy& copyInfo)
{
   if (dstBuffer == VK_NULL_HANDLE || srcBuffer == VK_NULL_HANDLE)
   {
      logger_.warn("Null buffers.");
      return;
   }

   if (recording_)
   {
      vkCmdCopyBuffer(commandBuffer_, srcBuffer, dstBuffer, 1U, &copyInfo);
   }
   else
   {
      logger_.warn("Command buffer not recording.");
   }
}

void CommandBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
{
   if (recording_)
   {
      vkCmdDraw(commandBuffer_, vertexCount, instanceCount, firstVertex, firstInstance);
   }
   else
   {
      logger_.warn("Command buffer not recording.");
   }
}

void CommandBuffer::endRecording()
{
   if (recording_)
   {
      VkResult errCode{ vkEndCommandBuffer(commandBuffer_) };
      if (errCode != VK_SUCCESS)
      {
         logger_.relFatal("Failed to complete the Vulkan command buffer recording.");
      }

      recording_ = false;
   }
}

void CommandBuffer::endRenderPass()
{
   if (recording_ && renderPassActive_)
   {
      vkCmdEndRenderPass(commandBuffer_);
      renderPassActive_ = false;
   }
   else
   {
      logger_.warn("Command buffer in the wrong state: recording = {}, renderPassActive = {}", recording_, renderPassActive_);
   }
}

void CommandBuffer::reset()
{
   VkResult errCode{ vkResetCommandBuffer(commandBuffer_, 0) };
   if (errCode != VK_SUCCESS)
   {
      logger_.relErr("Failed to reset command buffer: {}", errCode);
   }
}

void CommandBuffer::setScissor(const VkRect2D& scissor)
{
   vkCmdSetScissor(commandBuffer_, 0, 1, &scissor);
}

void CommandBuffer::setViewport(const VkViewport& viewport)
{
   vkCmdSetViewport(commandBuffer_, 0, 1, &viewport);
}