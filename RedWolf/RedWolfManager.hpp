#ifndef RW_REDWOLFMANAGER_HPP
#define RW_REDWOLFMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/VersionInfo.hpp"
#include "RedWolf/events/EventHandler.hpp"
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
         rw::dat::VersionInfo appVersion{ 0, 0, 0 };                            /**< Version of the user application. */
         unsigned int threadPoolThreads{ std::thread::hardware_concurrency() }; /**< Number of threads assigned to the thread pool. */
      };

      /**
       * @brief Constructor.
       * @param options Initialisation options for the library.
       */
      explicit RedWolfManager(const Options& options);

      /**
       * @brief Destructor.
       */
      ~RedWolfManager();

      /**
       * @brief Copy constructor.
       */
      RedWolfManager(const RedWolfManager&) = delete;

      /**
       * @brief Move constructor.
       */
      RedWolfManager(RedWolfManager&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      RedWolfManager& operator=(const RedWolfManager&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      RedWolfManager& operator=(RedWolfManager&&) = delete;

      /**
       * @brief Get the library event handler.
       * @return Library event handler.
       */
      [[nodiscard]] rw::events::EventHandler& eventHandler();

      /**
       * @brief Get the manager of the GLFW library.
       * @return Manager of the GLFW library.
       */
      [[nodiscard]] rw::lif::glfw::GlfwManager& glfwManager();

      /**
       * @brief Get the library logger.
       * @return Library logger.
       */
      [[nodiscard]] rw::utils::Logger& logger();

      /**
       * @brief Get the library settings manager.
       * @return Library settings manager.
       */
      [[nodiscard]] rw::utils::SettingsManager& settingsManager();

      /**
       * @brief Get the library thread pool.
       * @return Library thread pool.
       */
      [[nodiscard]] rw::thread::ThreadPool& threadPool();

      /**
       * @brief Get the Vulkan instance of the library.
       * @return Vulkan instance of the library.
       */
      [[nodiscard]] rw::lif::vlk::Instance& vulkanInstance();

      /**
       * @brief Get the Vulkan API interface.
       * @return Vulkan API interface.
       */
      [[nodiscard]] rw::lif::vlk::Interface& vulkanInterface();

   private:
      rw::utils::Logger          logger_;          /**< Logger for the library. */
      rw::utils::SettingsManager settingsManager_; /**< Settings manager for the library. */

      rw::thread::ThreadPool threadPool_; /**< Default thread pool for the library. */

      rw::events::EventHandler eventHandler_; /**< Event handler for the library. */

      rw::lif::glfw::GlfwManager glfwManager_;     /**< Manager for the GLFW library. */
      rw::lif::vlk::Interface    vulkanInterface_; /**< Interface to the Vulkan API. */

      rw::lif::vlk::Instance vulkanInstance_; /**< Vulkan instance used by the library. */
   };
} // namespace rw

#endif