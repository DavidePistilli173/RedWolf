#ifndef RW_UI_BASEWINDOW_HPP
#define RW_UI_BASEWINDOW_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/gfx/BaseEntity.hpp"
#include "RedWolf/gfx/Renderer.hpp"
#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/glfw/Window.hpp"

#include <atomic>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::ui
{
   /**
    * @brief Base class for the main window of a RedWolf application.
    */
   class RW_API BaseWindow : public rw::core::BaseObject
   {
   public:
      static constexpr std::string_view default_window_title{ "RedWolf Application" }; /**< Default window title. */
      static constexpr i32              default_window_width{ 800 };                   /**< Default window width. */
      static constexpr i32              default_window_height{ 600 };                  /**< Default window height.
                              
      /**
       * @brief Default constructor. The window is automatically opened.
       * @details This function must be called from the main thread.
       * @param manager RedWolf library manager.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       */
      BaseWindow(
         RedWolfManager&  manager,
         std::string_view title = default_window_title,
         i32              width = default_window_width,
         i32              height = default_window_height,
         bool             resizable = false);

      /**
       * @brief Destructor.
       */
      ~BaseWindow() override;

      /**
       * @brief Check window events.
       * @details This function must be called from the main thread.
       */
      void checkEvents();

      /**
       * @brief Callback for when the window is resized.
       * @param window Pointer to the window that was resized.
       * @param width New width of the window.
       * @param height New height of the window.
       */
      static void resizeCallback(GLFWwindow* window, i32 width, i32 height);

      rw::gfx::Renderer renderer_; /**< Window renderer. */

   private:
      /**
       * @brief Close the currently open window.
       * @details This function must be called from the main thread.
       */
      void close_();

      /**
       * @brief Open the window.
       * @details This function must be called from the main thread.
       * @return true on success, false otherwise.
       */
      bool open_();

      RedWolfManager&             manager_;     /**< RedWolf library manager. */
      rw::utils::Logger&          logger_;      /**< Library logger. */
      rw::lif::glfw::GlfwManager& glfwManager_; /**< Manager for the GLFW library. */

      rw::lif::glfw::Window window_;                       /**< Raw window. */
      std::thread           renderThread_;                 /**< Thread used for rendering. */
      std::atomic<bool>     renderThreadRunning_{ false }; /**< Loop control variable for the rendering thread. */

      std::vector<std::unique_ptr<rw::gfx::BaseEntity>> entities_; /**< Entities owned by this window. */
   };
} // namespace rw::ui

#endif