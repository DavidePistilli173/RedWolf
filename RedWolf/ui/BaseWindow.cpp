#include "BaseWindow.hpp"

#include "RedWolf/events/WindowCloseEvent.hpp"

namespace rw::ui
{
   BaseWindow::BaseWindow(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable, BaseObject* parent) :
      BaseObject(manager, parent), glfwManager_{ manager.glfwManager() }, window_{ manager, title, width, height, resizable }
   {
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
} // namespace rw::ui