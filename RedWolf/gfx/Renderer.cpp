#include "Renderer.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/DrawEvent.hpp"

using namespace rw::gfx;

Renderer::Renderer(RedWolfManager& manager) :
   BaseObject(manager), vulkanInterface_{ manager.vulkanInterface() }, vulkanInstance_{ manager.vulkanInstance() }
{
}

void Renderer::draw()
{
   std::scoped_lock lck{ drawMtx_ };

   commandBuffer_->beginRecording();
   // commandBuffer_->beginRenderPass(*renderPass_, frameBuffer_);

   for (const auto& pipeline : graphicsPipelines_)
   {
      vulkanInterface_.cmdBindPipeline(commandBuffer_->handle(), pipeline.handle(), VK_PIPELINE_BIND_POINT_GRAPHICS);
      generateEvent<rw::events::DrawEvent>(rw::events::DrawEvent(pipeline.id()), rw::events::EventHandler::GenerationPolicy::synchronous);
   }

   commandBuffer_->endRecording();
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
         done = true;
      }
   }

   // Create the command pool and command buffers.
   commandPool_ = std::make_unique<rw::lif::vlk::CommandPool>(manager_, *graphicsDevice_, rw::lif::vlk::QueueFamilies::Id::graphics);
   commandBuffer_ = std::make_unique<rw::lif::vlk::CommandBuffer>(manager_, *graphicsDevice_, *commandPool_);

   // Create the render pass.
   renderPass_ = std::make_unique<rw::lif::vlk::RenderPass>(manager_, *graphicsDevice_, *(surface_->swapChain()));

   // Initialise the framebuffers.
   for (const auto& image : surface_->swapChain()->images())
   {
      frameBuffers_.emplace_back(manager_, *graphicsDevice_, *renderPass_, image, *surface_);
   }

   // Create the default graphics pipelines.
   graphicsPipelines_.reserve(shader_map.size());
   for (const auto& shader : shader_map)
   {
      graphicsPipelines_.emplace_back(
         manager_,
         *graphicsDevice_,
         *(surface_->swapChain()),
         *renderPass_,
         shader.value.first,
         shader.value.second,
         static_cast<size_t>(shader.key));
   }
}

void Renderer::uninitialise()
{
   std::scoped_lock lck{ drawMtx_ };

   graphicsPipelines_.clear();
   renderPass_.reset();
   commandPool_.reset();
   graphicsDevice_.reset();
   physicalDevice_ = nullptr;
   surface_.reset();

   initialised_ = false;
}

void Renderer::userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) {}