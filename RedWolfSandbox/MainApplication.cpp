#include "MainApplication.hpp"

#include <RedWolf/utils/Settings.hpp>

MainApplication::MainApplication(int argc, char** argv) : BaseApplication(argc, argv), logger_{ rw::utils::Logger::instance() } {}

void MainApplication::userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender)
{
   switch (evnt.id)
   {
   case static_cast<unsigned char>(rw::events::BaseEvent::ReservedEventId::timeout):
      ++timerCount_;
      break;
   default:
      break;
   }
}

void MainApplication::userInit_()
{
   static constexpr std::string_view application_section{ "APPLICATION" };
   static constexpr std::string_view application_cycle_duration{ "cycle_duration" };
   static constexpr std::string_view application_total_iterations{ "total_iterations" };

   static constexpr std::string_view timer_section{ "TIMER" };
   static constexpr std::string_view timer_frequency{ "frequency" };

   rw::utils::Settings* settings{ rw::utils::Settings::instance() };

   // Set up the main loop.
   rw::utils::Settings::Node* applicationSettings{ settings->root()->child(application_section) };
   if (applicationSettings == nullptr)
   {
      logger_->relFatal("Failed to load application settings.");
   }

   setCycleDuration(std::chrono::seconds(std::atoi(applicationSettings->attribute(application_cycle_duration, "2").c_str())));
   totalIterations_ = std::atoi(applicationSettings->attribute(application_total_iterations, "5").c_str());

   // Set up the timer.
   rw::utils::Settings::Node* timerSettings{ settings->root()->child(timer_section) };
   if (timerSettings == nullptr)
   {
      logger_->relFatal("Failed to load timer settings.");
   }

   timer_.setFrequency(std::atof(timerSettings->attribute(timer_frequency, "10.0").c_str()));
   timer_.subscribe<rw::events::TimeoutEvent>(this);
   timer_.start();
}

void MainApplication::userRun_()
{
   ++mainLoopIteration_;
   logger_->relInfo("Iteration: {}, timer called {} times since the last update.", mainLoopIteration_, timerCount_.load());
   timerCount_ = 0U;

   if (mainLoopIteration_ > totalIterations_)
   {
      stop();
   }
   else if (mainLoopIteration_ == 10U)
   {
      timer_.unsubscribe<rw::events::TimeoutEvent>(this);
      logger_->info("Unsubscribed from timer.");
   }
   else if (mainLoopIteration_ == 20U)
   {
      timer_.subscribe<rw::events::TimeoutEvent>(this);
      logger_->info("Subscribed to timer.");
   }
   else if (mainLoopIteration_ == 30U)
   {
      timer_.stop();
      logger_->info("Timer stopped.");
   }
}