#include "MainApplication.hpp"

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
   setCycleDuration(std::chrono::seconds(1));
   timer_.setFrequency(60.0);
   timer_.subscribe<rw::events::TimeoutEvent>(this);
   timer_.start();
}

void MainApplication::userRun_()
{
   ++mainLoopIteration_;
   logger_->relInfo(
      "Main Application loop, iteration: {}, timer called {} times since the last update.", mainLoopIteration_, timerCount_.load());
   timerCount_ = 0U;

   if (mainLoopIteration_ == 10U)
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