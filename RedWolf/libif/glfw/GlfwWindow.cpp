#include "GlfwWindow.hpp"

namespace rw::libif
{
   GlfwWindow::GlfwWindow(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable) :
      logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }, title_{ title }, size_{ width, height }, resizable_{ resizable }
   {
   }

   GlfwWindow::~GlfwWindow()
   {
      close();
   }

   bool GlfwWindow::checkWindowClose() const
   {
      return glfwManager_.checkWindowClose(window_);
   }

   void GlfwWindow::close()
   {
      if (window_ != nullptr)
      {
         glfwManager_.destroyWindow(window_);
      }
   }

   bool GlfwWindow::open()
   {
      if (window_ != nullptr) return true;

      window_ = glfwManager_.createWindow(title_, size_.first, size_.second, resizable_);
      if (window_ == nullptr)
      {
         logger_.relErr("Failed to create window \"{}\" ({}x{}).", title_, size_.first, size_.second);
         return false;
      }

      return true;
   }

   bool GlfwWindow::isOpen() const
   {
      return window_ != nullptr;
   }
} // namespace rw::libif