#ifndef RW_GFX_RENDERER_HPP
#define RW_GFX_RENDERER_HPP

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/common.hpp"
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/lif/glfw/Window.hpp"
#include "RedWolf/lif/vulkan/CommandBuffer.hpp"
#include "RedWolf/lif/vulkan/CommandPool.hpp"
#include "RedWolf/lif/vulkan/Fence.hpp"
#include "RedWolf/lif/vulkan/FrameBuffer.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/GraphicsPipeline.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"
#include "RedWolf/lif/vulkan/Semaphore.hpp"
#include "RedWolf/lif/vulkan/SubmitPresentInfo.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

#include <array>
#include <memory>
#include <mutex>
#include <vector>

namespace rw::gfx
{
   class BaseEntity;
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
            shader_map{ { GfxPipelineId::plain, { "shaders/vertex/base.spv", "shaders/fragment/base.spv" } } };

      /**
       * @brief Stages in which command buffer execution will wait.
       */
      static constexpr std::array<VkPipelineStageFlags, 1U> wait_stages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      static constexpr size_t                               cmd_buff_num{ 1U };    /**< Number of command buffers per frame. */
      static constexpr size_t                               signal_sem_num{ 1U };  /**< Number of signal semaphores per frame. */
      static constexpr size_t                               swap_chains_num{ 1U }; /**< Number of swap chains to present the images to. */
      static constexpr size_t max_frames_in_flight{ 2U }; /**< Maximum number of rendered frames pending presentation. */

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
       * @brief Get the logical graphics device used by the renderer.
       * @return Logical graphics device used by the renderer.
       */
      const rw::lif::vlk::GraphicsDevice& graphicsDevice() const;

      /**
       * @brief Initialise the renderer for the given window
       * @return true on success, false otherwise.   .
       */
      bool initialise(const rw::lif::glfw::Window& window);

      /**
       * @brief Get the physical device used by the renderer.
       * @return Physical device used by the renderer.
       */
      const rw::lif::vlk::PhysicalDevice& physicalDevice() const;

      /**
       * @brief Register an entity to drawing operations.
       * @param entity Entity to register for drawing.
       */
      void registerEntity(BaseEntity* entity);

      /**
       * @brief Set the frame buffer of the renderer to be resized.
       */
      void resizeFrameBuffer();

      /**
       * @brief Uninitialise the renderer, freeing it for another initialisation.
       */
      void uninitialise();

      /**
       * @brief Unregister an entity from drawing operations.
       * @param entity Entity to unregister.
       */
      void unregisterEntity(BaseEntity* entity);

      /**
       * @brief Wait for the renderer to complete the last rendering operation. This is a blocking call.
       */
      void waitForCompletion();

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
      /**
       * @brief Create the graphics pipelines.
       */
      void createGraphicsPipelines_();

      /**
       * @brief Create the render pass.
       */
      void createRenderPass_();

      /**
       * @brief Initialise the frame buffers.
       */
      void initialiseFrameBuffers_();

      /**
       * @brief Recreate the swap chain due to a window resize event.
       */
      void recreateSwapChain_();

      /**
       * @brief Set the scissor data.
       */
      void setScissor_();

      /**
       * @brief Set the submission and presentation swapchain data.
       */
      void setSubmitPresentSwapChains_();

      /**
       * @brief Set the viewport data.
       */
      void setViewport_();

      std::mutex drawMtx_; /**< Mutex for protecting draw data. */

      bool initialised_{ false }; /**< If true, the renderer is already initialised. */

      rw::lif::vlk::Instance& vulkanInstance_; /**< Vulkan instance. */

      rw::lif::vlk::PhysicalDevice*                 physicalDevice_{ nullptr }; /**< Physical rendering device. */
      std::unique_ptr<rw::lif::vlk::GraphicsDevice> graphicsDevice_;            /**< Logical rendering device. */

      std::unique_ptr<rw::lif::vlk::Surface> surface_;      /**< Surface on which the window will be drawn. */
      std::vector<rw::lif::vlk::FrameBuffer> frameBuffers_; /**< Framebuffers for the swap chain images. */

      std::unique_ptr<rw::lif::vlk::CommandPool> renderCommandPool_;    /**< Pool of command buffers used for rendering. */
      std::vector<rw::lif::vlk::CommandBuffer>   renderCommandBuffers_; /**< Command buffers for rendering operations. */

      std::unique_ptr<rw::lif::vlk::RenderPass>   renderPass_;               /**< Render pass that will be used by the renderer. */
      std::vector<rw::lif::vlk::GraphicsPipeline> graphicsPipelines_;        /**< List of default graphics pipelines. */
      std::vector<rw::lif::vlk::Fence>            inFlightFences_;           /**< Fence to wait for the GPU. */
      std::vector<rw::lif::vlk::Semaphore>        imageAvailableSemaphores_; /**< Semaphore for swapchain image availability. */
      std::vector<rw::lif::vlk::Semaphore>        renderFinishedSemaphores_; /**< Semaphore for rendering operations completeness. */

      VkViewport viewport_{}; /**< Viewport data. */
      VkRect2D   scissor_{};  /**< Scissor data. */

      /**
       * @brief Submission and presentation parameters for the graphics queue.
       */
      std::array<rw::lif::vlk::SubmitPresentInfo<wait_stages.size(), cmd_buff_num, signal_sem_num, swap_chains_num>, max_frames_in_flight>
         submitPresentInfo_;

      size_t currentFrameIdx_{ 0U };       /**< Index of the frame currently being drawn. */
      bool   frameBufferResized_{ false }; /**< If true, a resize event has been signalled from outside. */

      std::unique_ptr<rw::lif::vlk::CommandPool>   copyCommandPool_;   /**< Pool of command buffers used for data copying. */
      std::unique_ptr<rw::lif::vlk::CommandBuffer> copyCommandBuffer_; /**< Command buffer used for data copying. */
      VkSubmitInfo                                 copySubmitInfo_{};  /**< Copy submission parameters. */
      VkCommandBuffer                              copyCommandBufferHandle_{ VK_NULL_HANDLE }; /**< Raw copy command buffer handle. */
      std::unique_ptr<rw::lif::vlk::Fence>         copyFence_;                                 /**< Fence for copy operations. */

      std::vector<BaseEntity*> entities_; /**< Entities to draw. */
   };
} // namespace rw::gfx

#endif