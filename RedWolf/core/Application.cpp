#include "Application.hpp"

using namespace rw::core;

Application::Application(i32 argc, char** argv)
{
   // Save the command-line arguments for later use, if present.
   if (argc > 0 && argv != nullptr)
   {
      arguments_.reserve(argc);
      for (size_t i = 0; i < argc; ++i)
      {
         arguments_.emplace_back(argv[i]);
      }
   }
}

void Application::init()
{
   userInit_();
}

void Application::run()
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

void Application::setCycleFrequency(f64 hertz)
{
   if (hertz >= 0.0F)
   {
      cycleDuration_ = std::chrono::microseconds(static_cast<long long>(1'000'000 / hertz));
   }
}

void Application::step()
{
   userRun_(); // Call the user-defined run function.
}

void Application::stop()
{
   running_ = false;
}

void Application::waitForNextCycle_(std::chrono::microseconds remainingTime)
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