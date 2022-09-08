#ifndef RW_LIBIF_GLFW_GLFWWINDOW_HPP
#define RW_LIBIF_GLFW_GLFWWINDOW_HPP

#include "RedWolf/libif/glfw/GlfwManager.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace rw::libif
{
   /**
    * @brief Wrapper for a GLFW window.
    */
   class GlfwWindow
   {
   public:
      /**
       * @brief Constructor. The window is NOT opened automatically.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       */
      GlfwWindow(std::string_view title, int width, int height, bool resizable);

      /**
       * @brief Destructor.
       */
      ~GlfwWindow();

      /**
       * @brief Check whether the window needs to be closed or not.
       * @return true if the window needs to be closed, false otherwise.
       */
      [[nodiscard]] bool checkWindowClose() const;

      /**
       * @brief Close the currently open window.
       */
      void close();

      /**
       * @brief Open the window.
       * @return true on success, false otherwise.
       */
      bool open();

      /**
       * @brief Check whether the window is open or not.
       * @return true if the window is open, false otherwise.
       */
      [[nodiscard]] bool isOpen() const;

   private:
      rw::utils::Logger* logger_; /**< Logger instance. */

      GlfwManager* glfwManager_{ nullptr }; /**< Manager for the GLFW library. */
      GLFWwindow*  window_{ nullptr };      /**< Window handle. */

      std::string         title_;              /**< Title of the window. */
      std::pair<int, int> size_{ 0, 0 };       /**< Size of the window. */
      bool                resizable_{ false }; /**< If true, the window is resizable. */
   };
} // namespace rw::libif

#endif