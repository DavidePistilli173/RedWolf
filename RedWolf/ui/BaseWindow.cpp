#include "BaseWindow.hpp"

#include "RedWolf/events/WindowCloseEvent.hpp"

namespace rw::ui
{
   BaseWindow::BaseWindow(std::string_view title, int width, int height, bool resizable, BaseObject* parent) : BaseObject(parent)
   {
      if (!openWindow(title, width, height, resizable))
      {
         throw std::exception{ "Failed to create window." };
      }
   }

   void BaseWindow::checkEvents()
   {
      if (glfwManager_.checkWindowClose())
      {
         closeWindow();
         generateEvent(rw::events::WindowCloseEvent());
      }

      glfwManager_.pollEvents();
   }

   void BaseWindow::closeWindow()
   {
      glfwManager_.closeWindow();
      isOpen_ = false;
   }

   bool BaseWindow::openWindow(std::string_view title, int width, int height, bool resizable)
   {
      if (isOpen_) closeWindow();
      if (glfwManager_.createWindow(title, width, height, resizable))
      {
         isOpen_ = true;
      }

      return isOpen_;
   }

   bool BaseWindow::isOpen() const
   {
      return isOpen_;
   }
} // namespace rw::ui