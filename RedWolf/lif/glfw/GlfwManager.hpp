#ifndef RW_LIF_GLFWMANAGER_HPP
#define RW_LIF_GLFWMANAGER_HPP

#include "RedWolf/common.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::utils
{
   class Logger;
} // namespace rw::utils

namespace rw::lif::glfw
{
   /**
    * @brief Interface for the GLFW library.
    * @details Each interface can support one window.
    */
   class RW_API GlfwManager
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      explicit GlfwManager(RedWolfManager& manager);

      /**
       * @brief Destructor.
       */
      ~GlfwManager();

      /**
       * @brief Copy constructor.
       */
      GlfwManager(const GlfwManager&) = delete;

      /**
       * @brief Move constructor.
       */
      GlfwManager(GlfwManager&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      GlfwManager& operator=(const GlfwManager&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      GlfwManager& operator=(GlfwManager&&) = delete;

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
      GLFWwindow* createWindow(std::string_view title, i32 width, i32 height, bool resizable);

      /**
       * @brief Create a Vulkan rendering surface associated with a Window.
       * @param vulkanInstance Instance of Vulkan.
       * @param window Window to create the surface for.
       * @return Vulkan rendering surface. Ownership goes to the caller.
       */
      VkSurfaceKHR createWindowSurface(VkInstance vulkanInstance, GLFWwindow* window);

      /**
       * @brief Destroy a window.
       * @param window Window to destroy.
       */
      void destroyWindow(GLFWwindow* window);

      /**
       * @brief Get the size of the frame buffer in pixels for a given window.
       * @param window Target window.
       * @return pair containing the width and the height, in this order.
       */
      [[nodiscard]] std::pair<i32, i32> getFrameBufferSize(GLFWwindow* window) const;

      /**
       * @brief Get the required Vulkan extensions for GLFW.
       * @return Vector containing the pointers to the names of all required extensions.
       */
      [[nodiscard]] std::vector<const char*> getRequiredVulkanInstanceExtensions();

      /**
       * @brief Get the user data associated with a given window.
       * @param window Window to get the user data for.
       */
      [[nodiscard]] void* getWindowUserPointer(GLFWwindow* window);

      /**
       * @brief Process all waiting events.
       * @details This function must be called from the main thread.
       */
      void pollEvents();

      /**
       * @brief Set the callback to call when a window is resized.
       * @param window Window to associate to the callback.
       * @param callback Callback to call when the specified window is resized.
       */
      void setFrameBufferResizeCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, i32, i32));

      /**
       * @brief Set the user data associated to a window.
       * @param window Window to associate the data to.
       * @param userData User data to associate to the window.
       */
      void setWindowUserPointer(GLFWwindow* window, void* userData);

      /**
       * @brief Wait for at least an event to happen and then process it.
       * @details Note that this call is blocking. and must be called from the main thread.
       */
      void waitEvents();

   private:
      rw::utils::Logger& logger_; /**< Logger instance. */
   };
}; // namespace rw::lif::glfw

#endif