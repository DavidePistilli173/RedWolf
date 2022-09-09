#ifndef RWSANDBOX_MAINWINDOW_HPP
#define RWSANDBOX_MAINWINDOW_HPP

#include <RedWolf/RedWolfManager.hpp>
#include <RedWolf/ui/BaseWindow.hpp>
#include <string_view>

class MainWindow : public rw::ui::BaseWindow
{
public:
   static constexpr std::string_view window_title{ "RedWolf Sandbox" }; /**< Title of the window. */
   static constexpr int              window_width{ 1366 };              /**< Width of the window. */
   static constexpr int              window_height{ 768 };              /**< Height of the window. */

   /**
    * @brief Constructor.
    * @param rw RedWolf library manager.
    */
   MainWindow(rw::RedWolfManager& rw);

protected:
   /**
    * @brief Implementation of the userHandle_ function from BaseObject.
    * @param evnt Event.
    * @param sender Sender of the event.
    */
   virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender) override;
};

#endif