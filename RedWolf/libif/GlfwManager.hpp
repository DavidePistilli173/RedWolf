#ifndef RW_LIBIF_GLFWMANAGER_HPP
#define RW_LIBIF_GLFWMANAGER_HPP

#include "RedWolf/utils/Logger.hpp"

#include <memory>
#include <mutex>
#include <string_view>

class GLFWwindow;

namespace rw::libif
{
   /**
    * @brief Interface for the GLFW library.
    * @detail Each interface can support one window.
    */
   class GlfwManager
   {
   public:
      /**
       * @brief Constructor.
       */
      GlfwManager();

      /**
       * @brief Destructor.
       */
      ~GlfwManager();

      /**
       * @brief Create a window.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @return true on success, false otherwise.
       */
      bool createWindow(std::string_view title, int width, int height, bool resizable);

      /**
       * @brief Check whether the currently open window should close or not.
       * @return true if the currently open window should close, false otherwise.
       */
      bool checkWindowClose();

      /**
       * @brief Close the currently open window.
       */
      void closeWindow();

      /**
       * @brief Process all waiting events.
       * @detail This function must be called from the main thread.
       */
      void pollEvents();

   private:
      /**
       * @brief Deleter for a GLFW window.
       * @param win Window to delete.
       */
      static void windowDeleter_(GLFWwindow* win);

      static std::mutex mtx_;         /**< Mutex for protecting concurrent access to the singleton. */
      static size_t     activeUsers_; /**< Number of currently active users. */

      rw::utils::Logger*                                 logger_; /**< Logger instance. */
      std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_; /**< Pointer to the window of the current GLFW manager. */
   };
}; // namespace rw::libif

#endif