#ifndef RW_LIF_VULKAN_RENDERPASS_HPP
#define RW_LIF_VULKAN_RENDERPASS_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

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
    * @brief Wrapper for a Vulkan VkRenderPass object.
    */
   class RW_API RenderPass : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Graphics device this render pass will be used on.
       * @param swapChain Swap chain this render pass will be used on.
       */
      RenderPass(RedWolfManager& manager, GraphicsDevice& device, SwapChain& swapChain);

      /**
       * @brief Destructor.
       */
      ~RenderPass();

      /**
       * @brief Copy constructor.
       */
      RenderPass(const RenderPass&) = delete;

      /**
       * @brief Move constructor.
       */
      RenderPass(RenderPass&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      RenderPass& operator=(const RenderPass&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      RenderPass& operator=(RenderPass&&) = delete;

      /**
       * @brief Get the raw handle to the render pass object.
       * @return Raw handle to the render pass object.
       */
      [[nodiscard]] VkRenderPass handle();

   private:
      VkRenderPass renderPass_{ VK_NULL_HANDLE }; /**< Raw handle to the render pass. */

      GraphicsDevice& graphicsDevice_; /**< Graphics device this render pass is bound to. */
   };
} // namespace rw::lif::vlk

#endif