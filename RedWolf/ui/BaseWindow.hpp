#ifndef RW_UI_BASEWINDOW_HPP
#define RW_UI_BASEWINDOW_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/libif/GlfwManager.hpp"

#include <atomic>
#include <mutex>
#include <string_view>

namespace rw::ui
{
   /**
    * @brief Base class for the main window of a RedWolf application.
    */
   class RW_API BaseWindow : public rw::core::BaseObject
   {
   public:
      static constexpr std::string_view default_window_title{ "RedWolf Application" }; /**< Default window title. */
      static constexpr int              default_window_width{ 800 };                   /**< Default window width. */
      static constexpr int              default_window_height{ 600 };                  /**< Default window height. */

      /**
       * @brief Default constructor.
       * @detail This function must be called from the main thread.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @param parent Parent of the window.
       */
      BaseWindow(
         std::string_view title = default_window_title,
         int              width = default_window_width,
         int              height = default_window_height,
         bool             resizable = false,
         BaseObject*      parent = nullptr);

      /**
       * @brief Destructor.
       */
      virtual ~BaseWindow() = default;

      /**
       * @brief Check window events.
       * @detail This function must be called from the main thread.
       */
      void checkEvents();

      /**
       * @brief Close the currently open window.
       * @detail This function must be called from the main thread.
       */
      void closeWindow();

      /**
       * @brief Open the window.
       * @detail This function must be called from the main thread.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @return true on success, false otherwise.
       */
      bool openWindow(
         std::string_view title = default_window_title,
         int              width = default_window_width,
         int              height = default_window_height,
         bool             resizable = false);

      /**
       * @brief Check whether the window is open or not.
       * @return true if the window is open, false otherwise.
       */
      [[nodiscard]] bool isOpen() const;

   private:
      rw::libif::GlfwManager glfwManager_;     /**< Manager for the GLFW library. */
      std::atomic<bool>      isOpen_{ false }; /**< True if the window is currently open. */
   };
} // namespace rw::ui

#endif