#include "BaseWindow.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/WindowCloseEvent.hpp"

using namespace rw::ui;

BaseWindow::BaseWindow(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable, BaseObject* parent) :
   BaseObject(manager, parent), manager_{ manager }, logger_{ manager.logger() },
   glfwManager_{ manager.glfwManager() }, window_{ manager, title, width, height, resizable }, vulkanInstance_{ manager.vulkanInstance() }
{
}

void BaseWindow::checkEvents()
{
   glfwManager_.pollEvents();

   if (window_.checkWindowClose())
   {
      close();
      generateEvent(rw::events::WindowCloseEvent());
   }
}

void BaseWindow::close()
{
   surface_.reset();
   graphicsDevice_.reset();
   window_.close();
}

bool BaseWindow::open()
{
   if (isOpen()) return true;

   if (!window_.open()) return false;

   // Create a surface.
   surface_ = std::make_unique<rw::lif::vlk::Surface>(manager_, window_.handle());

   // Get the physical device for rendering.
   bool done{ false };
   while (!done)
   {
      physicalDevice_ = vulkanInstance_.getNextPhysicalDevice(rw::lif::vlk::PhysicalDevice::OpCategory::graphics, physicalDevice_);
      if (physicalDevice_ == nullptr)
      {
         logger_.relErr("No physical device supports the graphics pipeline.");
      }

      graphicsDevice_ = physicalDevice_->createGraphicsDevice(surface_->handle());

      if (physicalDevice_->isSurfaceSupported(surface_->handle()) && surface_->setDevices(*physicalDevice_, *graphicsDevice_))
      {
         done = true;
      }
   }

   return true;
}

bool BaseWindow::isOpen() const
{
   return window_.isOpen();
}