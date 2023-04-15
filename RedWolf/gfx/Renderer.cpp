#include "Renderer.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/DrawEvent.hpp"
#include "RedWolf/gfx/BaseEntity.hpp"

using namespace rw::gfx;

Renderer::Renderer(RedWolfManager& manager) :
   BaseObject(manager), vulkanInstance_{ manager.vulkanInstance() }, submitPresentInfo_{
      rw::lif::vlk::SubmitPresentInfo<wait_stages.size(), cmd_buff_num, signal_sem_num, swap_chains_num>(manager),
      rw::lif::vlk::SubmitPresentInfo<wait_stages.size(), cmd_buff_num, signal_sem_num, swap_chains_num>(manager)
   }
{
}

void Renderer::draw()
{
   std::scoped_lock lck{ drawMtx_ };

   // Wait for the previous frame to finish rendering.
   inFlightFences_[currentFrameIdx_].wait();
   u32 imageIdx{ surface_->swapChain()->acquireNextImage(&imageAvailableSemaphores_[currentFrameIdx_]) };
   if (surface_->swapChain()->needsRecreation())
   {
      recreateSwapChain_();
      return;
   }
   inFlightFences_[currentFrameIdx_].reset();

   // Synchronise data between CPU and GPU.
   copyCommandBuffer_->reset();
   copyCommandBuffer_->beginRecording();
   for (auto entity : entities_)
   {
      entity->sync(*copyCommandBuffer_);
   }
   copyCommandBuffer_->endRecording();

   // Submit the copy command buffer and wait for its completion.
   if (vkQueueSubmit(graphicsDevice_->graphicsQueue(), 1U, &copySubmitInfo_, copyFence_->handle()) != VK_SUCCESS)
   {
      logger_.err("Failed to submit copy commands.");
   }
   copyFence_->waitAndReset();

   // Start recording the command buffer.
   renderCommandBuffers_[currentFrameIdx_].reset();
   renderCommandBuffers_[currentFrameIdx_].beginRecording();
   renderCommandBuffers_[currentFrameIdx_].beginRenderPass(renderPass_->handle(), frameBuffers_[imageIdx]);

   // Perform all draw calls for each pipeline.
   for (const auto& pipeline : graphicsPipelines_)
   {
      renderCommandBuffers_[currentFrameIdx_].bindPipeline(pipeline.handle(), VK_PIPELINE_BIND_POINT_GRAPHICS);
      renderCommandBuffers_[currentFrameIdx_].setViewport(viewport_);
      renderCommandBuffers_[currentFrameIdx_].setScissor(scissor_);

      for (auto& entity : entities_)
      {
         entity->draw(renderCommandBuffers_[currentFrameIdx_], pipeline.id());
      }
   }

   // Finish recording the command buffer.
   renderCommandBuffers_[currentFrameIdx_].endRenderPass();
   renderCommandBuffers_[currentFrameIdx_].endRecording();

   // Submit the graphics command buffer.
   if (
      vkQueueSubmit(
         graphicsDevice_->graphicsQueue(),
         1U,
         &submitPresentInfo_[currentFrameIdx_].submitInfo(),
         inFlightFences_[currentFrameIdx_].handle()) != VK_SUCCESS)
   {
      logger_.err("Failed to submit rendering commands.");
   }

   // Present the image.
   submitPresentInfo_[currentFrameIdx_].presentInfo().pImageIndices = &imageIdx;
   VkResult presentationResult{ vkQueuePresentKHR(
      graphicsDevice_->presentationQueue(), &submitPresentInfo_[currentFrameIdx_].presentInfo()) };

   // Check for framebuffer resizing.
   if (presentationResult == VK_ERROR_OUT_OF_DATE_KHR || presentationResult == VK_SUBOPTIMAL_KHR || frameBufferResized_)
   {
      frameBufferResized_ = false;
      recreateSwapChain_();
   }

   // Move to the next frame.
   currentFrameIdx_ = ++currentFrameIdx_ % max_frames_in_flight;
}

const rw::lif::vlk::GraphicsDevice& Renderer::graphicsDevice() const
{
   return *graphicsDevice_;
}

bool Renderer::initialise(const rw::lif::glfw::Window& window)
{
   std::scoped_lock lck{ drawMtx_ };

   if (initialised_) return false; // The renderer must first be uninitialised before initialising it again.

   // Create a surface.
   surface_ = std::make_unique<rw::lif::vlk::Surface>(manager_, window.handle());

   // Get the physical device for rendering.
   bool done{ false };
   while (!done)
   {
      physicalDevice_ = vulkanInstance_.getNextPhysicalDevice(rw::lif::vlk::PhysicalDevice::OpCategory::graphics, physicalDevice_);
      if (physicalDevice_ == nullptr)
      {
         logger_.relErr("No physical device supports the graphics pipeline.");
         return false;
      }

      if (physicalDevice_->isSurfaceSupported(surface_->handle()))
      {
         graphicsDevice_ = physicalDevice_->createGraphicsDevice();
         if (!surface_->setDevices(*physicalDevice_, *graphicsDevice_))
         {
            logger_.relErr("Failed to set rendering devices for the window surface.");
            return false;
         }
         VkPhysicalDeviceProperties properties{ physicalDevice_->properties() };
         logger_.info("Chosen GPU: {}", properties.deviceType);
         done = true;
      }
   }

   setSubmitPresentSwapChains_();

   // Create the rendering command pool and command buffers.
   renderCommandPool_ = std::make_unique<rw::lif::vlk::CommandPool>(manager_, *graphicsDevice_, rw::lif::vlk::QueueFamilies::Id::graphics);
   renderCommandBuffers_.reserve(max_frames_in_flight);
   for (size_t i = 0U; i < max_frames_in_flight; ++i)
   {
      std::array<VkCommandBuffer, cmd_buff_num> tmpCmdBuffers{
         renderCommandBuffers_.emplace_back(manager_, *graphicsDevice_, *renderCommandPool_).handle()
      };
      submitPresentInfo_[i].setCommandBuffers(tmpCmdBuffers);
   }

   // Create the render pass.
   createRenderPass_();

   // Initialise the framebuffers.
   initialiseFrameBuffers_();

   // Create the default graphics pipelines.
   createGraphicsPipelines_();

   // Initialise synchronisation objects.
   inFlightFences_.reserve(max_frames_in_flight);
   imageAvailableSemaphores_.reserve(max_frames_in_flight);
   renderFinishedSemaphores_.reserve(max_frames_in_flight);

   for (size_t i = 0U; i < max_frames_in_flight; ++i)
   {
      inFlightFences_.emplace_back(manager_, *graphicsDevice_);

      std::array<VkSemaphore, wait_stages.size()> tmpWaitSems{
         imageAvailableSemaphores_.emplace_back(manager_, *graphicsDevice_).handle()
      };
      submitPresentInfo_[i].setWaitSemaphores(tmpWaitSems, wait_stages);

      std::array<VkSemaphore, signal_sem_num> tmpSigSem{ renderFinishedSemaphores_.emplace_back(manager_, *graphicsDevice_).handle() };
      submitPresentInfo_[i].setSignalSemaphores(tmpSigSem);
   }

   // Initialise viewport and scissor data.
   setViewport_();
   setScissor_();

   // Create the copy command pool and command buffers.
   copyCommandPool_ = std::make_unique<rw::lif::vlk::CommandPool>(manager_, *graphicsDevice_, rw::lif::vlk::QueueFamilies::Id::graphics);
   copyCommandBuffer_ = std::make_unique<rw::lif::vlk::CommandBuffer>(manager_, *graphicsDevice_, *copyCommandPool_);
   copySubmitInfo_.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   copySubmitInfo_.commandBufferCount = 1U;
   copyCommandBufferHandle_ = copyCommandBuffer_->handle();
   copySubmitInfo_.pCommandBuffers = &copyCommandBufferHandle_;

   // Create the copy synchronisation objects.
   copyFence_ = std::make_unique<rw::lif::vlk::Fence>(manager_, *graphicsDevice_);

   return true;
}

const rw::lif::vlk::PhysicalDevice& Renderer::physicalDevice() const
{
   return *physicalDevice_;
}

void Renderer::registerEntity(BaseEntity* entity)
{
   std::scoped_lock lck{ drawMtx_ };

   auto it = std::find(entities_.begin(), entities_.end(), entity);
   if (it == entities_.end())
   {
      entities_.emplace_back(entity);
   }
}

void Renderer::resizeFrameBuffer()
{
   frameBufferResized_ = true;
}

void Renderer::uninitialise()
{
   std::scoped_lock lck{ drawMtx_ };

   copyFence_.reset();
   copyCommandPool_.reset();
   copyCommandBuffer_.reset();
   renderFinishedSemaphores_.clear();
   imageAvailableSemaphores_.clear();
   inFlightFences_.clear();
   graphicsPipelines_.clear();
   renderPass_.reset();
   renderCommandBuffers_.clear();
   renderCommandPool_.reset();
   frameBuffers_.clear();
   surface_.reset();
   graphicsDevice_.reset();
   physicalDevice_ = nullptr;

   initialised_ = false;
}

void Renderer::unregisterEntity(BaseEntity* entity)
{
   std::scoped_lock lck{ drawMtx_ };

   auto it = std::find(entities_.begin(), entities_.end(), entity);
   if (it != entities_.end())
   {
      if (it == entities_.end() - 1)
      {
         *it = *(entities_.end() - 1);
      }
      entities_.pop_back();
   }
}

void Renderer::waitForCompletion()
{
   graphicsDevice_->waitIdle();
}

void Renderer::userHandle_([[maybe_unused]] const rw::events::BaseEvent& evnt, [[maybe_unused]] const BaseObject* sender) {}

void Renderer::createGraphicsPipelines_()
{
   graphicsPipelines_.reserve(shader_map.size());
   for (const auto& shader : shader_map)
   {
      graphicsPipelines_.emplace_back(
         manager_, *graphicsDevice_, *renderPass_, shader.value.first, shader.value.second, static_cast<size_t>(shader.key));
   }
}

void Renderer::createRenderPass_()
{
   renderPass_ = std::make_unique<rw::lif::vlk::RenderPass>(manager_, *graphicsDevice_, *(surface_->swapChain()));
}

void Renderer::initialiseFrameBuffers_()
{
   for (const auto& image : surface_->swapChain()->images())
   {
      frameBuffers_.emplace_back(manager_, *graphicsDevice_, *renderPass_, image, *surface_);
   }
}

void Renderer::recreateSwapChain_()
{
   graphicsDevice_->waitIdle();

   // Clean-up.
   graphicsPipelines_.clear();
   frameBuffers_.clear();
   renderPass_.reset();

   // Swap chain recreation.
   surface_->setDevices(*physicalDevice_, *graphicsDevice_);
   setSubmitPresentSwapChains_();

   // Recreate the render pass.
   createRenderPass_();

   // Initialise the frame buffers.
   initialiseFrameBuffers_();

   // Recreate the graphics pipelines.
   createGraphicsPipelines_();

   // Initialise viewport and scissor data.
   setViewport_();
   setScissor_();
}

void Renderer::setScissor_()
{
   scissor_.offset = { 0, 0 };
   scissor_.extent = surface_->selectedExtent();
}

void Renderer::setSubmitPresentSwapChains_()
{
   std::array<VkSwapchainKHR, swap_chains_num> tmpSwapChains{ surface_->swapChain()->handle() };
   for (auto& info : submitPresentInfo_)
   {
      info.setSwapChains(tmpSwapChains);
   }
}

void Renderer::setViewport_()
{
   viewport_.x = 0.0F;
   viewport_.y = 0.0F;
   viewport_.width = static_cast<f32>(surface_->selectedExtent().width);
   viewport_.height = static_cast<f32>(surface_->selectedExtent().height);
   viewport_.minDepth = 0.0F;
   viewport_.maxDepth = 1.0F;
}