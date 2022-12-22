#ifndef RW_LIF_VULKAN_GRAPHICSPIPELINE_HPP
#define RW_LIF_VULKAN_GRAPHICSPIPELINE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/PipelineLayout.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"

#include <string_view>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class GraphicsDevice;
   class SwapChain;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkPipeline.
    */
   class RW_API GraphicsPipeline : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Graphics device this pipeline will be used on.
       * @param swapChain Swapchain this pipeline will be used on.
       * @param renderPass Render pass this pipeline will be used on.
       * @param vertShader Path to the compiled vertex shader.
       * @param fragShader Path to the compiled fragment shader.
       * @param id Pipeline ID.
       */
      GraphicsPipeline(
         RedWolfManager&   manager,
         GraphicsDevice&   device,
         SwapChain&        swapChain,
         const RenderPass& renderPass,
         std::string_view  vertShader,
         std::string_view  fragShader,
         size_t            id);

      /**
       * @brief Destructor.
       */
      ~GraphicsPipeline();

      /**
       * @brief Copy constructor.
       */
      GraphicsPipeline(const GraphicsPipeline&) = delete;

      /**
       * @brief Move constructor.
       */
      GraphicsPipeline(GraphicsPipeline&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

      /**
       * @brief Get the raw pipeline handle.
       * @return Raw pipeline handle.
       */
      [[nodiscard]] VkPipeline handle() const;

      /**
       * @brief Get the pipeline's ID.
       * @return Pipeline's ID.
       */
      [[nodiscard]] size_t id() const;

   private:
      VkPipeline   pipeline_{ VK_NULL_HANDLE }; /**< Raw pipeline handle. */
      const size_t id_{ 0U };                   /**< Pipeline ID. */

      GraphicsDevice& graphicsDevice_; /**< Graphics devic this pipeline is bound to. */
      PipelineLayout  layout_;         /**< Layout information for the pipeline. */
   };
} // namespace rw::lif::vlk

#endif