#ifndef RW_REDWOLFMANAGER_HPP
#define RW_REDWOLFMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/libif/glfw/GlfwManager.hpp"
#include "RedWolf/libif/vulkan/VulkanManager.hpp"
#include "RedWolf/utils/Logger.hpp"

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
         std::string appName;     /**< Name of the user's application. */
         int         appVerMajor; /**< Major version number of the user's application. */
         int         appVerMinor; /**< Minor version number of the user's application. */
         int         appVerPatch; /**< Patch version number of the user's application. */
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
      rw::libif::GlfwManager& glfwManager();

      /**
       * @brief Get the library logger.
       * @return Library logger.
       */
      rw::utils::Logger& logger();

      /**
       * @brief Get the manager of the Vulkan library.
       * @return Manager of the Vulkan library.
       */
      rw::libif::VulkanManager& vulkanManager();

   private:
      rw::utils::Logger logger_; /**< Logger for the library. */

      rw::libif::GlfwManager   glfwManager_;   /**< Manager for the GLFW library. */
      rw::libif::VulkanManager vulkanManager_; /**< Manager for the Vulkan library. */
   };
} // namespace rw

#endif