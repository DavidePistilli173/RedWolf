#include "BaseWindow.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/events/WindowCloseEvent.hpp"
#include "RedWolf/events/WindowResizeEvent.hpp"

using namespace rw::ui;

BaseWindow::BaseWindow(RedWolfManager& manager, std::string_view title, i32 width, i32 height, bool resizable) :
   BaseObject(manager), manager_{ manager }, logger_{ manager.logger() },
   glfwManager_{ manager.glfwManager() }, window_{ manager, title, width, height, resizable }, renderer_{ manager }
{
   if (!open_())
   {
      logger_.relFatal("Failed to open window.");
   }
}

BaseWindow::~BaseWindow()
{
   close_();

   // Stop the rendering thread.
   renderThreadRunning_ = false;
   if (renderThread_.joinable())
   {
      renderThread_.join();
   }
}

void BaseWindow::checkEvents()
{
   // glfwManager_.pollEvents();
   glfwManager_.waitEvents();

   if (window_.checkWindowClose())
   {
      generateEvent(rw::events::WindowCloseEvent());
   }
}

void BaseWindow::resizeCallback(GLFWwindow* window, i32 width, i32 height)
{
   // Raw GLFW call because the glfwManager instance is inaccessible at this point.
   auto thisPtr = reinterpret_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
   if (thisPtr == nullptr) return;

   thisPtr->renderer_.resizeFrameBuffer();
   thisPtr->generateEvent(rw::events::WindowResizeEvent(width, height));
}

void BaseWindow::close_()
{
   // Stop the rendering thread.
   renderThreadRunning_ = false;
   if (renderThread_.joinable())
   {
      renderThread_.join();
   }

   // De-initialise the window renderer.
   renderer_.uninitialise();

   // Destroy the GLFW window.
   window_.close();
}

bool BaseWindow::open_()
{
   if (window_.isOpen()) return true;

   // Create the GLFW window.
   if (!window_.open())
   {
      logger_.relErr("Failed to open GLFW window. ");
      return false;
   }

   // Initialise the window renderer.
   if (!renderer_.initialise(window_))
   {
      logger_.relErr("Failed to initialise window renderer.");
      window_.close();
      return false;
   }

   // Start the rendering thread.
   renderThreadRunning_ = true;
   renderThread_ = std::thread(
      [this]()
      {
         while (renderThreadRunning_)
         {
            renderer_.draw();
         }

         renderer_.waitForCompletion();
      });

   return true;
}