#include "MainApplication.hpp"

#include <RedWolf/events/DataReadyEvent.hpp>
#include <RedWolf/utils/Settings.hpp>

MainApplication::MainApplication(int argc, char** argv) :
   BaseGUIApplication(&mainWindow_, argc, argv), logger_{ rw::utils::Logger::instance() }, vulkanManager_{ "RedWolf Sandbox", 0, 5, 0 }
{
}

void MainApplication::userHandle_(const rw::events::BaseEvent& evnt, const BaseObject*)
{
   switch (evnt.id)
   {
   case static_cast<unsigned char>(rw::events::BaseEvent::ReservedEventId::timeout):
      ++timerCount_;
      break;
   case static_cast<unsigned char>(rw::events::BaseEvent::ReservedEventId::data_ready):
   {
      auto             eventData = reinterpret_cast<const rw::events::DataReadyEvent*>(&evnt);
      std::scoped_lock lck{ packetSenderMutex_ };
      senderAddress_ = eventData->address;
      senderPort_ = eventData->port;
      ++packetCount_;
   }
   break;
   default:
      break;
   }
}

void MainApplication::userInit_()
{
   static constexpr std::string_view application_section{ "APPLICATION" };
   static constexpr std::string_view application_cycle_frequency{ "cycle_frequency" };
   static constexpr std::string_view application_total_iterations{ "total_iterations" };

   static constexpr std::string_view timer_section{ "TIMER" };
   static constexpr std::string_view timer_frequency{ "frequency" };

   static constexpr std::string_view socket_section{ "SOCKET" };
   static constexpr std::string_view socket_local_address{ "local_address" };
   static constexpr std::string_view socket_local_port{ "local_port" };

   rw::utils::Settings* settings{ rw::utils::Settings::instance() };

   // Set up the main loop.
   rw::utils::Settings::Node* applicationSettings{ settings->root()->child(application_section) };
   if (applicationSettings == nullptr)
   {
      logger_->relFatal("Failed to load application settings.");
   }

   setCycleFrequency(std::atof(applicationSettings->attribute(application_cycle_frequency, "1.0").c_str()));
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

   // Set up the socket.
   rw::utils::Settings::Node* socketSettings{ settings->root()->child(socket_section) };
   if (socketSettings == nullptr)
   {
      logger_->relFatal("Failed to load socket settings.");
   }

   if (socket_.open(socketSettings->attribute(socket_local_address, "10.0.0.1"), socketSettings->attribute(socket_local_port, "4321")))
   {
      socket_.subscribe<rw::events::DataReadyEvent>(this);
   }
}

void MainApplication::userRun_()
{
   ++mainLoopIteration_;
   timerCount_ = 0U;

   if (mainLoopIteration_ > totalIterations_)
   {
      stop();
   }
   else if (mainLoopIteration_ == 100U)
   {
      timer_.unsubscribe<rw::events::TimeoutEvent>(this);
      logger_->info("Unsubscribed from timer.");
   }
   else if (mainLoopIteration_ == 200U)
   {
      timer_.subscribe<rw::events::TimeoutEvent>(this);
      logger_->info("Subscribed to timer.");
   }
   else if (mainLoopIteration_ == 300U)
   {
      timer_.stop();
      logger_->info("Timer stopped.");
   }

   // Send a packet back.
   if (packetCount_ > 0U)
   {
      std::scoped_lock lck{ packetSenderMutex_ };
      if (socket_.send(senderAddress_, senderPort_, "REPLY FROM REDWOLF APPLICATION!"))
      {
         logger_->info("Sent reply to {}:{}.", senderAddress_, senderPort_);
      }
      packetCount_ = 0U;
   }

   // Check whether the main window is open or not.
   if (!mainWindow_.isOpen())
   {
      stop();
   }
}