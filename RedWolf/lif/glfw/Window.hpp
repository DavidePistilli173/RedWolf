#ifndef RW_LIF_GLFW_WINDOW_HPP
#define RW_LIF_GLFW_WINDOW_HPP

#include "RedWolf/lif/glfw/GlfwManager.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace rw
{
   class RedWolfManager;
}

namespace rw::utils
{
   class Logger;
}

namespace rw::lif::glfw
{
   /**
    * @brief Wrapper for a GLFW window.
    */
   class Window
   {
   public:
      /**
       * @brief Constructor. The window is NOT opened automatically.
       * @param manager RedWolf library manager.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       */
      Window(RedWolfManager& manager, std::string_view title, int width, int height, bool resizable);

      /**
       * @brief Destructor.
       */
      ~Window();

      /**
       * @brief Copy constructor.
       */
      Window(const Window&) = delete;

      /**
       * @brief Move constructor.
       * @param other Window to move from.
       */
      Window(Window&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      Window& operator=(const Window&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Window& operator=(Window&&) = delete;

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
       * @brief Get the raw window handle.
       * @return Raw window handle.
       */
      [[nodiscard]] GLFWwindow* handle() const;

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
      rw::utils::Logger& logger_;      /**< Logger instance. */
      GlfwManager&       glfwManager_; /**< Manager for the GLFW library. */

      GLFWwindow* window_{ nullptr }; /**< Window handle. */

      std::string         title_;              /**< Title of the window. */
      std::pair<int, int> size_{ 0, 0 };       /**< Size of the window. */
      bool                resizable_{ false }; /**< If true, the window is resizable. */
   };
} // namespace rw::lif::glfw

#endif