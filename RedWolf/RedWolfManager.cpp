#include "RedWolfManager.hpp"

namespace rw
{
   RedWolfManager::RedWolfManager(const Options& options) :
      logger_{}, settingsManager_{ *this }, threadPool_{ *this, options.threadPoolThreads }, glfwManager_{ *this }, vulkanManager_{
         *this, options.appName, options.appVerMajor, options.appVerMinor, options.appVerPatch
      }
   {
   }

   rw::libif::GlfwManager& RedWolfManager::glfwManager()
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

   rw::libif::VulkanManager& RedWolfManager::vulkanManager()
   {
      return vulkanManager_;
   }
} // namespace rw