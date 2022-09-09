#include "RedWolfManager.hpp"

namespace rw
{
   RedWolfManager::RedWolfManager(const Options& options) :
      logger_{}, glfwManager_{ *this }, vulkanManager_{
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

   rw::libif::VulkanManager& RedWolfManager::vulkanManager()
   {
      return vulkanManager_;
   }
} // namespace rw