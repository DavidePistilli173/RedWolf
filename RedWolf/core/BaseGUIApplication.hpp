#ifndef RW_CORE_BASEGUIAPPLICATION_HPP
#define RW_CORE_BASEGUIAPPLICATION_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseApplication.hpp"
#include "RedWolf/ui/BaseWindow.hpp"

#include <mutex>
#include <optional>
#include <vector>

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
       * @param window Pointer to a window of the application.
       * @param argc Number of command line arguments.
       * @param argv List of command line arguments.
       */
      BaseGUIApplication(BaseWindow* window = nullptr, int argc = 0, char** argv = nullptr);

      /**
       * @brief Add a new window to the application.
       * @param window New window to add.
       */
      void addWindow(BaseWindow* window);

      /**
       * @brief Run the application.
       */
      virtual void run() override;

   protected:
   private:
      /**
       * @brief Update all the windows associated with the current application.
       */
      void updateWindows_();

      std::mutex               windowMtx_; /**< Mutex for protecting windows access. */
      std::vector<BaseWindow*> windows_;   /**< List of all windows associated with this application. */
   };
} // namespace rw::core

#endif