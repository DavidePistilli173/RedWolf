#include "BaseGUIApplication.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/ui/BaseWindow.hpp"

using namespace rw::core;

BaseGUIApplication::BaseGUIApplication(RedWolfManager& manager, int argc, char** argv) : BaseApplication(manager, argc, argv) {}

void BaseGUIApplication::run()
{
   running_ = true;
   while (running_)
   {
      // Get the start time of the current loop.
      auto startTime = std::chrono::high_resolution_clock::now();

      // Poll events for all windows.
      updateWindows_();

      BaseApplication::step(); // Step the main application.

      // Get the end time of the current loop
      auto endTime = std::chrono::high_resolution_clock::now();

      // Sleep/wait for the remaining time if sleeping is enabled.
      auto remainingTime{ cycleDuration_ - std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime) };
      if (remainingTime > std::chrono::microseconds(0))
      {
         waitForNextCycle_(remainingTime);
      }
   }
}

void BaseGUIApplication::updateWindows_()
{
   std::scoped_lock lck{ windowMtx_ };
   for (auto& win : windows_)
   {
      win->checkEvents();
   }
}