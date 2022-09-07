#include "GlfwManager.hpp"

#include "RedWolf/common.hpp"

#include <GLFW/glfw3.h>

namespace rw::libif
{
   std::mutex GlfwManager::mtx_;               /**< Mutex for protecting concurrent access to the singleton. */
   size_t     GlfwManager::activeUsers_{ 0U }; /**< Number of currently active users. */

   GlfwManager::GlfwManager() : logger_{ rw::utils::Logger::instance() }, window_{ nullptr, GlfwManager::windowDeleter_ }
   {
      std::scoped_lock lck{ mtx_ };
      if (activeUsers_ == 0U)
      {
         glfwInit();
         glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Required to disable OpenGL in GLFW.
      }
      ++activeUsers_;
   }

   GlfwManager::~GlfwManager()
   {
      std::scoped_lock lck{ mtx_ };
      --activeUsers_;
      if (activeUsers_ == 0U) glfwTerminate();
   }

   bool GlfwManager::createWindow(std::string_view title, int width, int height, bool resizable)
   {
      if (resizable)
      {
         glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
      }
      else
      {
         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      }

      window_ = std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>(
         glfwCreateWindow(width, height, title.data(), nullptr, nullptr), GlfwManager::windowDeleter_);
      if (window_ == nullptr)
      {
         logger_->relErr("Failed to create window \"{}\" ({}x{}).", title, width, height);
         return false;
      }

      return true;
   }

   bool GlfwManager::checkWindowClose()
   {
      return glfwWindowShouldClose(window_.get());
   }

   void GlfwManager::closeWindow()
   {
      window_.reset();
   }

   void GlfwManager::pollEvents()
   {
      glfwPollEvents();
   }

   void GlfwManager::windowDeleter_(GLFWwindow* win)
   {
      glfwDestroyWindow(win);
   }
} // namespace rw::libif