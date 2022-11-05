#include "RedWolfManager.hpp"

using namespace rw;

RedWolfManager::RedWolfManager(const Options& options) :
   logger_{}, settingsManager_{ *this }, threadPool_{ *this, options.threadPoolThreads }, eventHandler_{ *this }, glfwManager_{ *this },
   vulkanInterface_{
      *this,
   },
   vulkanInstance_{ *this, options.appName, options.appVersion, glfwManager_.getRequiredVulkanInstanceExtensions() }
{
}

RedWolfManager::~RedWolfManager()
{
   threadPool_.stopAllThreads();
}

rw::events::EventHandler& RedWolfManager::eventHandler()
{
   return eventHandler_;
}

rw::lif::glfw::GlfwManager& RedWolfManager::glfwManager()
{
   return glfwManager_;
}

rw::utils::Logger& RedWolfManager::logger()
{
   return logger_;
}

rw::utils::SettingsManager& RedWolfManager::settingsManager()
{
   return settingsManager_;
}

rw::thread::ThreadPool& RedWolfManager::threadPool()
{
   return threadPool_;
}

rw::lif::vlk::Instance& RedWolfManager::vulkanInstance()
{
   return vulkanInstance_;
}

rw::lif::vlk::Interface& RedWolfManager::vulkanInterface()
{
   return vulkanInterface_;
}