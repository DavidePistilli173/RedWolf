#ifndef RWSANDBOX_MAINWINDOW_HPP
#define RWSANDBOX_MAINWINDOW_HPP

#include <RedWolf/ui/Window.hpp>
#include <string_view>

namespace rwsandbox
{
   /**< \brief Main window for the application. */
   class MainWindow : public rw::ui::Window
   {
   public:
      /**< Name of the main window. */
      static constexpr std::string_view win_name{ "Sandbox!!!" };

      /**< Construct a window with the default settings. */
      MainWindow();

      /**< Main loop of the application. */
      bool run() override;

   private:
   };
} // namespace rwsandbox

#endif