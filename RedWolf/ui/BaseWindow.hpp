#ifndef RW_UI_BASEWINDOW_HPP
#define RW_UI_BASEWINDOW_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/glfw/Window.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/GraphicsPipeline.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

#include <atomic>
#include <memory>
#include <mutex>
#include <string_view>
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
      static constexpr int              default_window_width{ 800 };                   /**< Default window width. */
      static constexpr int              default_window_height{ 600 };                  /**< Default window height. */

      /**
       * @brief Different graphics pipelines.
       */
      enum class GfxPipelineIdx
      {
         plain
      };

      /**
       * @brief Map for associating graphics pipelines with their source shaders.
       */
      static constexpr rw::cont::ConstexprMap<GfxPipelineIdx, std::pair<std::string_view, std::string_view>, 4U> shader_map{
         { GfxPipelineIdx::plain, { "shaders/vertex/base.vert", "shaders/vertex/base.frag" } }
      };

      /**
       * @brief Default constructor. The window is NOT automatically opened.
       * @details This function must be called from the main thread.
       * @param manager RedWolf library manager.
       * @param title Title of the window.
       * @param width Width of the window.
       * @param height Height of the window.
       * @param resizable If true, the window will be resizable.
       * @param parent Parent of the window.
       */
      BaseWindow(
         RedWolfManager&  manager,
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
       * @details This function must be called from the main thread.
       */
      void checkEvents();

      /**
       * @brief Close the currently open window.
       * @details This function must be called from the main thread.
       */
      void close();

      /**
       * @brief Open the window.
       * @details This function must be called from the main thread.
       * @return true on success, false otherwise.
       */
      bool open();

      /**
       * @brief Check whether the window is open or not.
       * @return true if the window is open, false otherwise.
       */
      [[nodiscard]] bool isOpen() const;

   private:
      RedWolfManager&             manager_;     /**< RedWolf library manager. */
      rw::utils::Logger&          logger_;      /**< Library logger. */
      rw::lif::glfw::GlfwManager& glfwManager_; /**< Manager for the GLFW library. */

      rw::lif::glfw::Window window_; /**< Raw window. */

      rw::lif::vlk::Instance&                       vulkanInstance_;            /**< Vulkan instance. */
      rw::lif::vlk::PhysicalDevice*                 physicalDevice_{ nullptr }; /**< Physical rendering device. */
      std::unique_ptr<rw::lif::vlk::GraphicsDevice> graphicsDevice_;            /**< Logical rendering device. */
      std::unique_ptr<rw::lif::vlk::Surface>        surface_;                   /**< Surface on which the window will be drawn. */
      std::vector<rw::lif::vlk::GraphicsPipeline>   graphicsPipelines_;         /**< List of default graphics pipelines. */
   };
} // namespace rw::ui

#endif