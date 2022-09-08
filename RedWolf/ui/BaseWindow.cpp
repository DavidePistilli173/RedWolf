#include "BaseWindow.hpp"

#include "RedWolf/events/WindowCloseEvent.hpp"

namespace rw::ui
{
   BaseWindow::BaseWindow(std::string_view title, int width, int height, bool resizable, BaseObject* parent) :
      BaseObject(parent), glfwManager_{ rw::libif::GlfwManager::instance() }, window_{ title, width, height, resizable }
   {
   }

   void BaseWindow::checkEvents()
   {
      if (window_.checkWindowClose())
      {
         close();
         generateEvent(rw::events::WindowCloseEvent());
      }

      glfwManager_->pollEvents();
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