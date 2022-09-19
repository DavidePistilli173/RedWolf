#include "BaseWindow.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/WindowCloseEvent.hpp"

using namespace rw::ui;

BaseWindow::BaseWindow(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable, BaseObject* parent) :
   BaseObject(manager, parent), logger_{ manager.logger() },
   glfwManager_{ manager.glfwManager() }, window_{ manager, title, width, height, resizable }, vulkanInstance_{ manager.vulkanInstance() }
{
   // Create a surface.
   surface_ = std::make_unique<rw::lif::vlk::Surface>(manager, window_.handle());

   // Get the physical device for rendering.
   bool done{ false };
   while (!done)
   {
      physicalDevice_ = vulkanInstance_.getNextPhysicalDevice(rw::lif::vlk::PhysicalDevice::OpCategory::graphics, physicalDevice_);
      if (physicalDevice_ == nullptr)
      {
         logger_.relFatal("No physical device supports the graphics pipeline.");
      }

      if (physicalDevice_->isSurfaceSupported(surface_->handle()) && surface_->init(physicalDevice_->handle()))
      {
         done = true;
      }
   }
}

void BaseWindow::checkEvents()
{
   if (window_.checkWindowClose())
   {
      close();
      generateEvent(rw::events::WindowCloseEvent());
   }

   glfwManager_.pollEvents();
}

void BaseWindow::close()
{
   window_.close();
}

bool BaseWindow::open(std::string_view title, int width, int height, bool resizable)
{
   return window_.open();
}

bool BaseWindow::isOpen() const
{
   return window_.isOpen();
}