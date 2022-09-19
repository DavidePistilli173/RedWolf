#ifndef RW_REDWOLFMANAGER_HPP
#define RW_REDWOLFMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/VersionInfo.hpp"
#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/thread/ThreadPool.hpp"
#include "RedWolf/utils/Logger.hpp"
#include "RedWolf/utils/SettingsManager.hpp"

#include <string>

namespace rw
{
   /**
    * @brief Class for managing library resources and dependencies.
    */
   class RW_API RedWolfManager
   {
   public:
      /**
       * @brief Options to be passed to the manager.
       */
      struct Options
      {
         std::string          appName;                                          /**< Name of the user application. */
         rw::dat::VersionInfo appVersion;                                       /**< Version of the user application. */
         unsigned int threadPoolThreads{ std::thread::hardware_concurrency() }; /**< Number of threads assigned to the thread pool. */
      };

      /**
       * @brief Constructor.
       * @param options Initialisation options for the library.
       */
      RedWolfManager(const Options& options);

      /**
       * @brief Get the manager of the GLFW library.
       * @return Manager of the GLFW library.
       */
      rw::lif::glfw::GlfwManager& glfwManager();

      /**
       * @brief Get the library logger.
       * @return Library logger.
       */
      rw::utils::Logger& logger();

      /**
       * @brief Get the library settings manager.
       * @return Library settings manager.
       */
      rw::utils::SettingsManager& settingsManager();

      /**
       * @brief Get the library thread pool.
       * @return Library thread pool.
       */
      rw::thread::ThreadPool& threadPool();

      /**
       * @brief Get the Vulkan instance of the library.
       * @return Vulkan instance of the library.
       */
      rw::lif::vlk::Instance& vulkanInstance();

      /**
       * @brief Get the Vulkan API interface.
       * @return Vulkan API interface.
       */
      rw::lif::vlk::Interface& vulkanInterface();

   private:
      rw::utils::Logger          logger_;          /**< Logger for the library. */
      rw::utils::SettingsManager settingsManager_; /**< Settings manager for the library. */

      rw::thread::ThreadPool threadPool_; /**< Default thread pool for the library. */

      rw::lif::glfw::GlfwManager glfwManager_;     /**< Manager for the GLFW library. */
      rw::lif::vlk::Interface    vulkanInterface_; /**< Interface to the Vulkan API. */

      rw::lif::vlk::Instance vulkanInstance_; /**< Vulkan instance used by the library. */
   };
} // namespace rw

#endif