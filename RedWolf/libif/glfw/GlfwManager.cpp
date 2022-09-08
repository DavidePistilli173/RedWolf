#include "GlfwManager.hpp"

namespace rw::libif
{
   std::mutex                                           GlfwManager::mtx_;
   std::unique_ptr<GlfwManager, void (*)(GlfwManager*)> GlfwManager::instance_{ nullptr, nullptr };

   GlfwManager* GlfwManager::instance()
   {
      std::scoped_lock lck{ mtx_ };
      if (instance_ == nullptr)
      {
         instance_ = std::unique_ptr<GlfwManager, void (*)(GlfwManager*)>(new GlfwManager(), [](GlfwManager* manager) { delete manager; });
      }
      return instance_.get();
   }

   bool GlfwManager::checkWindowClose(GLFWwindow* window)
   {
      return glfwWindowShouldClose(window);
   }

   GLFWwindow* GlfwManager::createWindow(std::string_view title, int width, int height, bool resizable)
   {
      if (resizable)
      {
         glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
      }
      else
      {
         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      }

      return glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
   }

   void GlfwManager::destroyWindow(GLFWwindow* window)
   {
      glfwDestroyWindow(window);
   }

   std::vector<const char*> GlfwManager::getRequiredVulkanInstanceExtensions()
   {
      uint32_t     glfwExtensionCount{ 0U };
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      std::vector<const char*> retVal;
      for (uint32_t i{ 0U }; i < glfwExtensionCount; ++i)
      {
         retVal.emplace_back(glfwExtensions[i]);
      }

      return retVal;
   }

   void GlfwManager::pollEvents()
   {
      glfwPollEvents();
   }

   GlfwManager::GlfwManager() : logger_{ rw::utils::Logger::instance() }
   {
      if (glfwInit() == GLFW_FALSE)
      {
         logger_->relFatal("Failed to initialise GLFW.");
      }
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Required to disable OpenGL in GLFW.
      logger_->relInfo("GLFW initialised.");
   }

   GlfwManager::~GlfwManager()
   {
      glfwTerminate();
      logger_->relInfo("GLFW cleaned-up.");
   }
} // namespace rw::libif