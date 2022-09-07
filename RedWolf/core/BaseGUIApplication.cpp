#include "BaseGUIApplication.hpp"

namespace rw::core
{
   BaseGUIApplication::BaseGUIApplication(BaseWindow* window, int argc, char** argv) : BaseApplication(argc, argv)
   {
      if (window != nullptr)
      {
         windows_.emplace_back(window);
      }
   }

   void BaseGUIApplication::addWindow(BaseWindow* window)
   {
      if (window != nullptr)
      {
         std::scoped_lock lck{ windowMtx_ };
         windows_.emplace_back(window);
      }
   }

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

         // Sleep for the remaining time if sleeping is enabled.
         if (cycleDuration_ != cycle_control_value_)
         {
            std::this_thread::sleep_for(cycleDuration_ - std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime));
         }
      }
   }

   void BaseGUIApplication::updateWindows_()
   {
      std::scoped_lock lck{ windowMtx_ };
      for (auto win : windows_)
      {
         win->checkEvents();
      }
   }
} // namespace rw::core