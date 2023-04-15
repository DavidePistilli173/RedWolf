#include "MainApplication.hpp"

#include <RedWolf/events/DataReadyEvent.hpp>
#include <RedWolf/events/WindowCloseEvent.hpp>
#include <RedWolf/utils/SettingsManager.hpp>

MainApplication::MainApplication(rw::RedWolfManager& rw, int argc, char** argv) :
   BaseGUIApplication(rw, argc, argv), rw_{ rw }, logger_{ rw.logger() }
{
   mainWindow_ = addWindow<MainWindow>(rw);
   mainWindow_->subscribe<rw::events::WindowCloseEvent>(this);
}

void MainApplication::userHandle_(const rw::events::BaseEvent& evnt, const BaseObject*)
{
   switch (static_cast<rw::events::BaseEvent::ReservedEventId>(evnt.id))
   {
   case rw::events::BaseEvent::ReservedEventId::window_close:
      removeWindow(mainWindow_);
      break;
   }
}

void MainApplication::userInit_()
{
   static constexpr std::string_view application_section{ "APPLICATION" };
   static constexpr std::string_view application_cycle_frequency{ "cycle_frequency" };
   static constexpr std::string_view application_total_iterations{ "total_iterations" };

   rw::utils::SettingsManager& settings{ rw_.settingsManager() };

   // Set up the main loop.
   rw::utils::SettingsManager::Node* applicationSettings{ settings.get()->child(application_section) };
   if (applicationSettings == nullptr)
   {
      logger_.relFatal("Failed to load application settings.");
   }

   setCycleFrequency(std::atof(applicationSettings->attribute(application_cycle_frequency, "1.0").c_str()));
   totalIterations_ = std::atoi(applicationSettings->attribute(application_total_iterations, "5").c_str());
}

void MainApplication::userRun_() {}