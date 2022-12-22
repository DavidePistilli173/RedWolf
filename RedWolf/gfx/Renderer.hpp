#ifndef RW_GFX_RENDERER_HPP
#define RW_GFX_RENDERER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/lif/glfw/Window.hpp"
#include "RedWolf/lif/vulkan/CommandBuffer.hpp"
#include "RedWolf/lif/vulkan/CommandPool.hpp"
#include "RedWolf/lif/vulkan/FrameBuffer.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/GraphicsPipeline.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

#include <memory>
#include <mutex>
#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::gfx
{
   /**
    * @brief Graphics renderer.
    */
   class RW_API Renderer : public rw::core::BaseObject
   {
   public:
      /**
       * @brief IDs of the default graphics pipelines.
       */
      enum class GfxPipelineId
      {
         plain,
         num_reserved
      };

      /**
       * @brief Map for associating default graphics pipelines with their source shaders.
       */
      static constexpr rw::cont::
         ConstexprMap<GfxPipelineId, std::pair<std::string_view, std::string_view>, static_cast<size_t>(GfxPipelineId::num_reserved)>
            shader_map{ { GfxPipelineId::plain, { "shaders/vertex/base.spv", "shaders/vertex/base.spv" } } };

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      Renderer(RedWolfManager& manager);

      /**
       * @brief Destructor.
       */
      ~Renderer() override = default;

      /**
       * @brief Copy constructor.
       */
      Renderer(const Renderer&) = delete;

      /**
       * @brief Move constructor.
       */
      Renderer(Renderer&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      Renderer& operator=(const Renderer&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Renderer& operator=(Renderer&&) = delete;

      /**
       * @brief Draw a new frame.
       */
      void draw();

      /**
       * @brief Initialise the renderer for the given window
       * @return true on success, false otherwise.   .
       */
      bool initialise(const rw::lif::glfw::Window& window);

      /**
       * @brief Uninitialise the renderer, freeing it for another initialisation.
       */
      void uninitialise();

   protected:
      /**
       * @brief User handle function for incoming events. To be implemented in derived classes.
       * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because
       * events could be generated from different threads (eg. rw::time::Timer).
       * @param evnt Event data.
       * @param sender Sender of the event.
       */
      void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) override;

   private:
      std::mutex drawMtx_; /**< Mutex for protecting draw data. */

      bool initialised_{ false }; /**< If true, the renderer is already initialised. */

      rw::lif::vlk::Interface& vulkanInterface_; /**< Interface to the Vulkan API. */

      rw::lif::vlk::Instance&                vulkanInstance_; /**< Vulkan instance. */
      std::unique_ptr<rw::lif::vlk::Surface> surface_;        /**< Surface on which the window will be drawn. */
      std::vector<rw::lif::vlk::FrameBuffer> frameBuffers_;   /**< Framebuffers for the swap chain images. */

      rw::lif::vlk::PhysicalDevice*                 physicalDevice_{ nullptr }; /**< Physical rendering device. */
      std::unique_ptr<rw::lif::vlk::GraphicsDevice> graphicsDevice_;            /**< Logical rendering device. */

      std::unique_ptr<rw::lif::vlk::CommandPool>   commandPool_;   /**< Pool of command buffers for the renderer. */
      std::unique_ptr<rw::lif::vlk::CommandBuffer> commandBuffer_; /**< Command buffer for rendering operations. */

      std::unique_ptr<rw::lif::vlk::RenderPass>   renderPass_;        /**< Render pass that will be used by the renderer. */
      std::vector<rw::lif::vlk::GraphicsPipeline> graphicsPipelines_; /**< List of default graphics pipelines. */
   };
} // namespace rw::gfx

#endif