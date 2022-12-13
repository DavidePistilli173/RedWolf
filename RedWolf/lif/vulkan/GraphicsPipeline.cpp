#include "GraphicsPipeline.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/ShaderModule.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"

#include <array>
#include <vector>

using namespace rw::lif::vlk;

GraphicsPipeline::GraphicsPipeline(
   RedWolfManager& manager, GraphicsDevice& device, SwapChain& swapChain, std::string_view vertShader, std::string_view fragShader) :
   BaseObject(manager),
   graphicsDevice_{ device }, layout_{ manager, device }, renderPass_{ manager, device, swapChain }
{
   // Load shaders.
   ShaderModule                    vertexShader(manager, device, vertShader);
   VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
   vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
   vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
   vertShaderStageInfo.module = vertexShader.handle();
   vertShaderStageInfo.pName = ShaderModule::entry_point_name.data();

   ShaderModule                    fragmentShader(manager, device, fragShader);
   VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
   fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
   fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
   fragShaderStageInfo.module = fragmentShader.handle();
   fragShaderStageInfo.pName = ShaderModule::entry_point_name.data();

   std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

   // Vertex input description.
   VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
   vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
   vertexInputInfo.vertexBindingDescriptionCount = 0U;
   vertexInputInfo.pVertexBindingDescriptions = nullptr;
   vertexInputInfo.vertexAttributeDescriptionCount = 0U;
   vertexInputInfo.pVertexAttributeDescriptions = nullptr;

   // Input assembly description.
   VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
   inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
   inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
   inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

   // Dynamic state description.
   std::vector<VkDynamicState>      dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
   VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
   dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
   dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
   dynamicStateInfo.pDynamicStates = dynamicStates.data();

   // Viewport and scissor description.
   VkPipelineViewportStateCreateInfo viewportInfo{};
   viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
   viewportInfo.viewportCount = 1U;
   viewportInfo.scissorCount = 1U;

   // Rasterizer description.
   VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
   rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
   rasterizerInfo.depthClampEnable = VK_FALSE;
   rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
   rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
   rasterizerInfo.lineWidth = 1.0F;
   rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
   rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
   rasterizerInfo.depthBiasEnable = VK_FALSE;
   rasterizerInfo.depthBiasConstantFactor = 0.0F;
   rasterizerInfo.depthBiasClamp = 0.0F;
   rasterizerInfo.depthBiasSlopeFactor = 0.0F;

   // Multisampling description.
   VkPipelineMultisampleStateCreateInfo multisamplingInfo{};
   multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
   multisamplingInfo.sampleShadingEnable = VK_FALSE;
   multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
   multisamplingInfo.minSampleShading = 1.0F;
   multisamplingInfo.pSampleMask = nullptr;
   multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
   multisamplingInfo.alphaToOneEnable = VK_FALSE;

   // Colour blending description.
   VkPipelineColorBlendAttachmentState colourBlendAttachmentInfo{};
   colourBlendAttachmentInfo.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
   colourBlendAttachmentInfo.blendEnable = VK_TRUE;
   colourBlendAttachmentInfo.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
   colourBlendAttachmentInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   colourBlendAttachmentInfo.colorBlendOp = VK_BLEND_OP_ADD;
   colourBlendAttachmentInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   colourBlendAttachmentInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
   colourBlendAttachmentInfo.alphaBlendOp = VK_BLEND_OP_ADD;

   VkPipelineColorBlendStateCreateInfo colourBlendInfo{};
   colourBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
   colourBlendInfo.logicOpEnable = VK_FALSE;
   colourBlendInfo.logicOp = VK_LOGIC_OP_COPY;
   colourBlendInfo.attachmentCount = 1U;
   colourBlendInfo.pAttachments = &colourBlendAttachmentInfo;
   colourBlendInfo.blendConstants[0] = 0.0F;
   colourBlendInfo.blendConstants[1] = 0.0F;
   colourBlendInfo.blendConstants[2] = 0.0F;
   colourBlendInfo.blendConstants[3] = 0.0F;

   // Pipeline creation.
   VkGraphicsPipelineCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
   createInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
   createInfo.pStages = shaderStages.data();
   createInfo.pVertexInputState = &vertexInputInfo;
   createInfo.pInputAssemblyState = &inputAssemblyInfo;
   createInfo.pViewportState = &viewportInfo;
   createInfo.pRasterizationState = &rasterizerInfo;
   createInfo.pMultisampleState = &multisamplingInfo;
   createInfo.pDepthStencilState = nullptr;
   createInfo.pColorBlendState = &colourBlendInfo;
   createInfo.pDynamicState = &dynamicStateInfo;
   createInfo.layout = layout_.handle();
   createInfo.renderPass = renderPass_.handle();
   createInfo.subpass = 0U;
   createInfo.basePipelineHandle = VK_NULL_HANDLE;
   createInfo.basePipelineIndex = -1;

   pipeline_ = vulkanInterface_.createGraphicsPipeline(graphicsDevice_.handle(), createInfo);
}

GraphicsPipeline::~GraphicsPipeline()
{
   vulkanInterface_.destroyPipeline(graphicsDevice_.handle(), pipeline_);
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept :
   BaseObject(other.manager_), pipeline_{ other.pipeline_ }, graphicsDevice_{ other.graphicsDevice_ }, layout_{ std::move(other.layout_) },
   renderPass_{ std::move(other.renderPass_) }
{
   other.pipeline_ = VK_NULL_HANDLE;
}

VkPipeline GraphicsPipeline::handle() const
{
   return pipeline_;
}