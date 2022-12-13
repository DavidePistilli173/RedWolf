#include "RenderPass.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"

using namespace rw::lif::vlk;

RenderPass::RenderPass(RedWolfManager& manager, GraphicsDevice& device, SwapChain& swapChain) :
   BaseObject(manager), graphicsDevice_{ device }
{
   VkAttachmentDescription colourAttachmentDescription{};
   colourAttachmentDescription.format = swapChain.format().format;
   colourAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
   colourAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
   colourAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
   colourAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
   colourAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   colourAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   colourAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

   VkAttachmentReference colourAttachmentRef{};
   colourAttachmentRef.attachment = 0U;
   colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

   VkSubpassDescription subpassDescription{};
   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
   subpassDescription.colorAttachmentCount = 1U;
   subpassDescription.pColorAttachments = &colourAttachmentRef;

   VkRenderPassCreateInfo renderPassInfo{};
   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
   renderPassInfo.attachmentCount = 1U;
   renderPassInfo.pAttachments = &colourAttachmentDescription;
   renderPassInfo.subpassCount = 1U;
   renderPassInfo.pSubpasses = &subpassDescription;

   renderPass_ = vulkanInterface_.createRenderPass(graphicsDevice_.handle(), renderPassInfo);
   if (renderPass_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create render pass.");
   }
}

RenderPass::~RenderPass()
{
   vulkanInterface_.destroyRenderPass(graphicsDevice_.handle(), renderPass_);
}

RenderPass::RenderPass(RenderPass&& other) noexcept :
   BaseObject(other.manager_), renderPass_{ other.renderPass_ }, graphicsDevice_{ other.graphicsDevice_ }
{
   other.renderPass_ = VK_NULL_HANDLE;
}

VkRenderPass RenderPass::handle() const
{
   return renderPass_;
}