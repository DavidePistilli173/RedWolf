#include "RenderPass.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"

using namespace rw::lif::vlk;

RenderPass::RenderPass(RedWolfManager& manager, const DeviceBase& device, const SwapChain& swapChain) :
   BaseObject(manager), device_{ device }
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

   VkSubpassDependency subpassDependency{};
   subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Implicit subpass before the render pass.
   subpassDependency.dstSubpass = 0;                   // Our render pass.
   subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
   subpassDependency.srcAccessMask = 0;
   subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
   subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

   VkRenderPassCreateInfo renderPassInfo{};
   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
   renderPassInfo.attachmentCount = 1U;
   renderPassInfo.pAttachments = &colourAttachmentDescription;
   renderPassInfo.subpassCount = 1U;
   renderPassInfo.pSubpasses = &subpassDescription;
   renderPassInfo.dependencyCount = 1U;
   renderPassInfo.pDependencies = &subpassDependency;

   renderPass_ = device_.createRenderPass(renderPassInfo);
   if (renderPass_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create render pass.");
   }
}

RenderPass::~RenderPass()
{
   device_.destroyRenderPass(renderPass_);
}

RenderPass::RenderPass(RenderPass&& other) noexcept : BaseObject(other.manager_), renderPass_{ other.renderPass_ }, device_{ other.device_ }
{
   other.renderPass_ = VK_NULL_HANDLE;
}

VkRenderPass RenderPass::handle() const
{
   return renderPass_;
}