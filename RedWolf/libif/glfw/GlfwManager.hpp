#ifndef RW_LIBIF_GLFWMANAGER_HPP
#define RW_LIBIF_GLFWMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>

namespace rw::libif
{
   /**
    * @brief Interface for the GLFW library.
    * @detail Each interface can support one window.
    */
   class RW_API GlfwManager
   {
   public:
      /**
       * @brief Get the instance of the GLFW library manager.
       */
      [[nodiscard]] static GlfwManager* instance();

      /**
       * @brief Check whether a window should close or not.
       * @param window Window to check.
       * @return true if the currently open window should close, false otherwise.
       */
      bool checkWindowClose(GLFWwindow* window);

      /**
       * @brief Create a window.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @return true on success, false otherwise.
       */
      GLFWwindow* createWindow(std::string_view title, int width, int height, bool resizable);

      /**
       * @brief Destroy a window.
       * @param window Window to destroy.
       */
      void destroyWindow(GLFWwindow* window);

      /**
       * @brief Get the required Vulkan extensions for GLFW.
       * @return Vector containing the pointers to the names of all required extensions.
       */
      [[nodiscard]] std::vector<const char*> getRequiredVulkanInstanceExtensions();

      /**
       * @brief Process all waiting events.
       * @detail This function must be called from the main thread.
       */
      void pollEvents();

   private:
      /**
       * @brief Constructor.
       */
      GlfwManager();

      /**
       * @brief Destructor.
       */
      ~GlfwManager();

      static std::mutex                                           mtx_;      /**< Mutex for protecting access to the instance. */
      static std::unique_ptr<GlfwManager, void (*)(GlfwManager*)> instance_; /**< Instance of the manager. */

      rw::utils::Logger* logger_; /**< Logger instance. */
   };
}; // namespace rw::libif

#endif