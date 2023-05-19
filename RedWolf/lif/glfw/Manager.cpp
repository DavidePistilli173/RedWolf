#include "Manager.hpp"

#include <GLFW/glfw3.h>

using namespace rw::lif::glfw;

Manager::Manager(rw::util::Logger& logger) : logger_{ logger }
{
   if (glfwInit() == GLFW_FALSE)
   {
      logger_.relFatal("Failed to initialise GLFW.");
   }
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Required to disable OpenGL in GLFW.
   logger_.relInfo("GLFW initialised.");
}

Manager::~Manager()
{
   glfwTerminate();
   logger_.relInfo("GLFW cleaned-up.");
}