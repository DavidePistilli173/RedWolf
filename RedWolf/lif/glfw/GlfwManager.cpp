#include "GlfwManager.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::glfw;

GlfwManager::GlfwManager(RedWolfManager& manager) : logger_{ manager.logger() }
{
   if (glfwInit() == GLFW_FALSE)
   {
      logger_.relFatal("Failed to initialise GLFW.");
   }
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Required to disable OpenGL in GLFW.
   logger_.relInfo("GLFW initialised.");
}

GlfwManager::~GlfwManager()
{
   glfwTerminate();
   logger_.relInfo("GLFW cleaned-up.");
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

VkSurfaceKHR GlfwManager::createWindowSurface(VkInstance vulkanInstance, GLFWwindow* window)
{
   if (vulkanInstance == VK_NULL_HANDLE || window == nullptr)
   {
      logger_.relErr("Null Vulkan or window handle.");
      return VK_NULL_HANDLE;
   }

   VkSurfaceKHR result;

   if (glfwCreateWindowSurface(vulkanInstance, window, nullptr, &result) != VK_SUCCESS)
   {
      logger_.relErr("Failed to create window surface.");
   }

   return result;
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