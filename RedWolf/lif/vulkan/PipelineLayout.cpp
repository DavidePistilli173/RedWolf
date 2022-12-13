#include "PipelineLayout.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

using namespace rw::lif::vlk;

PipelineLayout::PipelineLayout(RedWolfManager& manager, GraphicsDevice& device) : BaseObject(manager), device_{ device }
{
   VkPipelineLayoutCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   createInfo.setLayoutCount = 0U;
   createInfo.pSetLayouts = nullptr;
   createInfo.pushConstantRangeCount = 0;
   createInfo.pPushConstantRanges = nullptr;

   pipelineLayout_ = vulkanInterface_.createPipelineLayout(device_.handle(), createInfo);
   if (pipelineLayout_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create pipeline layout.");
   }
}

PipelineLayout::~PipelineLayout()
{
   vulkanInterface_.destroyPipelineLayout(device_.handle(), pipelineLayout_);
}

PipelineLayout::PipelineLayout(PipelineLayout&& other) noexcept :
   BaseObject(other.manager_), pipelineLayout_{ other.pipelineLayout_ }, device_{ other.device_ }
{
   other.pipelineLayout_ = VK_NULL_HANDLE;
}

VkPipelineLayout PipelineLayout::handle() const
{
   return pipelineLayout_;
}