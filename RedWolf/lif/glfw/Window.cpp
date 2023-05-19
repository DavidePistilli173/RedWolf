#include "Window.hpp"

using namespace rw::lif::glfw;

Window::Window(rw::util::Logger& logger, std::string_view title, i32 width, i32 height, bool resizable, void* userData) :
   logger_{ logger }, userData_{ userData }, title_{ title }, size_{ width, height }, resizable_{ resizable }
{
}

Window::~Window()
{
   close();
}

Window::Window(Window&& other) noexcept :
   logger_{ other.logger_ }, title_{ other.title_ }, size_{ other.size_ }, resizable_{ other.resizable_ }
{
   window_ = other.window_;
   other.window_ = nullptr;
}

bool Window::checkWindowClose() const
{
   return glfwWindowShouldClose(window_);
}

void Window::close()
{
   glfwDestroyWindow(window_);
   window_ = nullptr;
}

GLFWwindow* Window::handle() const
{
   return window_;
}

bool Window::open()
{
   if (window_ != nullptr) return true;

   if (resizable_)
   {
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
   }
   else
   {
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
   }

   window_ = glfwCreateWindow(size_.first, size_.second, title_.data(), nullptr, nullptr);
   if (window_ == nullptr)
   {
      logger_.relErr("Failed to create window \"{}\" ({}x{}).", title_, size_.first, size_.second);
      return false;
   }

   glfwSetWindowUserPointer(window_, userData_);
   glfwSetFramebufferSizeCallback(window_, resizeCallback_);

   return true;
}

bool Window::isOpen() const
{
   return window_ != nullptr;
}

void Window::setResizeCallback(void (*callback)(GLFWwindow*, i32, i32))
{
   resizeCallback_ = callback;
   if (open()) glfwSetFramebufferSizeCallback(window_, callback);
}