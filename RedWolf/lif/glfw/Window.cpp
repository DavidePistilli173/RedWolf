#include "Window.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::glfw;

Window::Window(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable) :
   logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }, title_{ title }, size_{ width, height }, resizable_{ resizable }
{
}

Window::~Window()
{
   close();
}

Window::Window(Window&& other) :
   logger_{ other.logger_ }, glfwManager_{ other.glfwManager_ }, title_{ other.title_ }, size_{ other.size_ }, resizable_{
      other.resizable_
   }
{
   window_ = other.window_;
   other.window_ = nullptr;
}

Window& Window::operator=(Window&& other)
{
   window_ = other.window_;
   other.window_ = nullptr;

   title_ = other.title_;
   size_ = other.size_;
   resizable_ = other.resizable_;

   return *this;
}

bool Window::checkWindowClose() const
{
   return glfwManager_.checkWindowClose(window_);
}

void Window::close()
{
   if (window_ != nullptr)
   {
      glfwManager_.destroyWindow(window_);
   }
}

GLFWwindow* Window::handle()
{
   return window_;
}

bool Window::open()
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

bool Window::isOpen() const
{
   return window_ != nullptr;
}