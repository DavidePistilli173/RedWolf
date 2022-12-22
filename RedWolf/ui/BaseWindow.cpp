#include "BaseWindow.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/WindowCloseEvent.hpp"

using namespace rw::ui;

BaseWindow::BaseWindow(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable, BaseObject* parent) :
   BaseObject(manager), manager_{ manager }, logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }, window_{
      manager, title, width, height, resizable
   } //, vulkanInstance_{ manager.vulkanInstance() }
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
   // surface_.reset();
   // graphicsDevice_.reset();
   window_.close();
}

bool BaseWindow::open()
{
   if (isOpen()) return true;

   if (!window_.open()) return false;

   // if (!renderer_.initialise())
   //{
   //    window_.close();
   //    return false;
   // }

   return true;
}

bool BaseWindow::isOpen() const
{
   return window_.isOpen();
}