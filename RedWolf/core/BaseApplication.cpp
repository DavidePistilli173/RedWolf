#include "BaseApplication.hpp"

namespace rw::core
{
   BaseApplication::BaseApplication(int argc, char** argv) : logger_{ rw::utils::Logger::instance() }
   {
      // Save the command-line arguments for later use, if present.
      if (argc > 0 && argv != nullptr)
      {
         logger_->trace("BaseApplication::BaseApplication - Saving {} command-line arguments.", argc);

         arguments_.reserve(argc);
         for (int i = 0; i < argc; ++i)
         {
            arguments_.emplace_back(argv[i]);
         }
      }
   };

   void BaseApplication::init()
   {
      logger_->trace("BaseApplication::init - Initialising.");

      userInit_();

      logger_->trace("BaseApplication::init - Init complete.");
   }

   void BaseApplication::run()
   {
      running_ = true;
      while (running_)
      {
         // Get the start time of the current loop.
         auto startTime = std::chrono::high_resolution_clock::now();

         userRun_(); // Call the user-defined run function.

         // Get the end time of the current loop
         auto endTime = std::chrono::high_resolution_clock::now();

         // Sleep for the remaining time if sleeping is enabled.
         if (cycleDuration_ != cycle_control_value_)
         {
            std::this_thread::sleep_for(cycleDuration_ - std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime));
         }
      }
   }

   void BaseApplication::setCycleFrequency(double hertz)
   {
      if (hertz >= 0.0F)
      {
         cycleDuration_ = std::chrono::microseconds(static_cast<long long>(1'000'000 / hertz));
         logger_->trace(
            "BaseApplication::setCycleFrequency - Main loop frequency set to {}Hz (cycleDuration_ = {} microseconds)",
            hertz,
            cycleDuration_);
      }
      else
      {
         logger_->err("BaseApplication::setCycleFrequency - Main loop frequency must be a positive float or 0, but {} was passed.", hertz);
      }
   }

   void BaseApplication::stop()
   {
      running_ = false;
      logger_->trace("BaseApplication::stop - Main loop stopped.");
   }
} // namespace rw::core