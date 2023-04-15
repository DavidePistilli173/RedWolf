#include "Window.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::glfw;

Window::Window(RedWolfManager& manager, std::string_view title, i32 width, i32 height, bool resizable, void* userData) :
   logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }, userData_{ userData }, title_{ title }, size_{ width, height },
   resizable_{ resizable }
{
}

Window::~Window()
{
   close();
}

Window::Window(Window&& other) noexcept :
   logger_{ other.logger_ }, glfwManager_{ other.glfwManager_ }, title_{ other.title_ }, size_{ other.size_ }, resizable_{
      other.resizable_
   }
{
   window_ = other.window_;
   other.window_ = nullptr;
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
      window_ = nullptr;
   }
}

GLFWwindow* Window::handle() const
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

   glfwManager_.setWindowUserPointer(window_, userData_);
   glfwManager_.setFrameBufferResizeCallback(window_, resizeCallback_);

   return true;
}

bool Window::isOpen() const
{
   return window_ != nullptr;
}

void Window::setResizeCallback(void (*callback)(GLFWwindow*, i32, i32))
{
   resizeCallback_ = callback;
   if (open()) glfwManager_.setFrameBufferResizeCallback(window_, callback);
}