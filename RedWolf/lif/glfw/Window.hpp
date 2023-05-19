#ifndef RW_LIF_GLFW_WINDOW_HPP
#define RW_LIF_GLFW_WINDOW_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/util/Logger.hpp"

#include <GLFW/glfw3.h>
#include <string_view>

namespace rw::lif::glfw
{
   /**
    * @brief Wrapper for a GLFWwindow.
    */
   class RW_API Window
   {
   public:
      /**
       * @brief Constructor. The window is NOT opened automatically.
       * @param logger Logger.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @param userData Data pointer to use in GLFW callbacks.
       */
      Window(rw::util::Logger& logger, std::string_view title, i32 width, i32 height, bool resizable, void* userData = nullptr);

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

      /**
       * @brief Set the callback to call when the window is resized.
       * @param callback Callback to call when the window is resized.
       */
      void setResizeCallback(void (*callback)(GLFWwindow*, i32, i32));

   private:
      rw::util::Logger& logger_; /**< Logger instance. */

      GLFWwindow* window_{ nullptr }; /**< Window handle. */

      std::string         title_;              /**< Title of the window. */
      std::pair<i32, i32> size_{ 0, 0 };       /**< Size of the window. */
      bool                resizable_{ false }; /**< If true, the window is resizable. */

      void* userData_{ nullptr };                                /**< User data associated with the window. */
      void (*resizeCallback_)(GLFWwindow*, i32, i32){ nullptr }; /**< Window resize callback. */
   };
} // namespace rw::lif::glfw

#endif