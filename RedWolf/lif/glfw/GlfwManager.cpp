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

GLFWwindow* GlfwManager::createWindow(std::string_view title, i32 width, i32 height, bool resizable)
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

std::pair<i32, i32> GlfwManager::getFrameBufferSize(GLFWwindow* window) const
{
   std::pair<i32, i32> result;

   if (window != nullptr)
   {
      glfwGetFramebufferSize(window, &result.first, &result.second);
   }

   return result;
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

void* GlfwManager::getWindowUserPointer(GLFWwindow* window)
{
   if (window == nullptr)
   {
      logger_.relErr("Cannot get the user data for a null window.");
      return nullptr;
   }

   return glfwGetWindowUserPointer(window);
}

void GlfwManager::pollEvents()
{
   glfwPollEvents();
}

void GlfwManager::setFrameBufferResizeCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, i32, i32))
{
   if (window == nullptr)
   {
      logger_.relErr("Cannot set the resize callback for a null window.");
      return;
   }

   glfwSetFramebufferSizeCallback(window, callback);
}

void GlfwManager::setWindowUserPointer(GLFWwindow* window, void* userData)
{
   if (window == nullptr)
   {
      logger_.relErr("Cannot set the user data for a null window.");
      return;
   }

   glfwSetWindowUserPointer(window, userData);
}

void GlfwManager::waitEvents()
{
   glfwWaitEvents();
}