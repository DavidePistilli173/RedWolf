#ifndef RW_CORE_BASEGUIAPPLICATION_HPP
#define RW_CORE_BASEGUIAPPLICATION_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseApplication.hpp"

#include <memory>
#include <mutex>
#include <optional>
#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::ui
{
   class BaseWindow;
}

namespace rw::core
{
   /**
    * @brief Base class for all GUI applications.
    */
   class RW_API BaseGUIApplication : public BaseApplication
   {
   public:
      using BaseWindow = rw::ui::BaseWindow;

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param argc Number of command line arguments.
       * @param argv List of command line arguments.
       */
      BaseGUIApplication(RedWolfManager& manager, int argc = 0, char** argv = nullptr);

      /**
       * @brief Destructor.
       */
      ~BaseGUIApplication() override = default;

      /**
       * @brief Copy constructor.
       */
      BaseGUIApplication(const BaseGUIApplication&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseGUIApplication(BaseGUIApplication&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      BaseGUIApplication& operator=(const BaseGUIApplication&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      BaseGUIApplication& operator=(BaseGUIApplication&&) = delete;

      /**
       * @brief Create a new window of a specific type and add it to the application.
       * @tparam Window Type of window to add. Must inherit from BaseWindow.
       * @tparam Args Argument types for the window's constructor.
       * @param args Arguments for the window's constructor.
       * @return Non-owning pointer to the newly created window.
       */
      template<typename Window, typename... Args>
      Window* makeWindow(Args&&... args)
      {
         std::scoped_lock lck{ windowMtx_ };
         return dynamic_cast<Window*>(windows_.emplace_back(std::make_unique<Window>(std::forward<Args>(args)...)).get());
      }

      /**
       * @brief Run the application.
       */
      void run() override;

   protected:
   private:
      /**
       * @brief Update all the windows associated with the current application.
       */
      void updateWindows_();

      std::mutex                               windowMtx_; /**< Mutex for protecting windows access. */
      std::vector<std::unique_ptr<BaseWindow>> windows_;   /**< List of all windows associated with this application. */
   };
} // namespace rw::core

#endif