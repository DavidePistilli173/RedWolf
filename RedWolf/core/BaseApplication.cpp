#include "BaseApplication.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::core;

BaseApplication::BaseApplication(RedWolfManager& manager, i32 argc, char** argv) : BaseObject(manager), logger_{ manager.logger() }
{
   // Save the command-line arguments for later use, if present.
   if (argc > 0 && argv != nullptr)
   {
      logger_.trace("Saving {} command-line arguments.", argc);

      arguments_.reserve(argc);
      for (size_t i = 0; i < argc; ++i)
      {
         arguments_.emplace_back(argv[i]);
      }
   }
};

void BaseApplication::init()
{
   userInit_();
}

void BaseApplication::run()
{
   running_ = true;
   while (running_)
   {
      // Get the start time of the current loop.
      auto startTime = std::chrono::high_resolution_clock::now();

      step();

      // Get the end time of the current loop
      auto endTime = std::chrono::high_resolution_clock::now();

      // Sleep for the remaining time if sleeping is enabled.
      auto remainingTime{ cycleDuration_ - std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime) };
      if (remainingTime > std::chrono::microseconds(0))
      {
         waitForNextCycle_(remainingTime);
      }
   }
}

void BaseApplication::setCycleFrequency(f64 hertz)
{
   if (hertz >= 0.0F)
   {
      cycleDuration_ = std::chrono::microseconds(static_cast<long long>(1'000'000 / hertz));
      logger_.trace("Main loop frequency set to {}Hz (cycleDuration_ = {})", hertz, cycleDuration_);
   }
   else
   {
      logger_.err("Main loop frequency must be a positive float or 0, but {} was passed.", hertz);
   }
}

void BaseApplication::step()
{
   userRun_(); // Call the user-defined run function.
}

void BaseApplication::stop()
{
   running_ = false;
   logger_.trace("Main loop stopped.");
}

void BaseApplication::waitForNextCycle_(std::chrono::microseconds remainingTime)
{
   if (remainingTime < busy_waiting_threshold)
   {
      // Busy waiting.
      auto startTime = std::chrono::high_resolution_clock::now();

      while (cycleDuration_ - std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime) >
             std::chrono::microseconds(0))
      {
      }
   }
   else
   {
      std::this_thread::sleep_for(remainingTime);
   }
}