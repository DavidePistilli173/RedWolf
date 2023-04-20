#ifndef RW_CORE_APPLICATION_HPP
#define RW_CORE_APPLICATION_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/Object.hpp"
#include "RedWolf/time/concepts.hpp"

#include <chrono>

namespace rw::core
{
   /**
    * @brief Base class for non-GUI applications.
    */
   class RW_API Application : public Object
   {
   public:
      /**
       * @brief Lower main loop intervals will be dealt with through busy waiting.
       */
      static constexpr std::chrono::microseconds busy_waiting_threshold{ 50000 };

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param argc Number of command line arguments.
       * @param argv List of command line arguments.
       */
      explicit Application(i32 argc = 0, char** argv = nullptr);

      /**
       * @brief Destructor.
       */
      ~Application() override = default;

      /**
       * @brief Base applications cannot be copied.
       */
      Application(const Application&) = delete;

      /**
       * @brief Move constructor.
       */
      Application(Application&& other) = default;

      /**
       * @brief Base applications cannot be copied
       */
      Application& operator=(const Application&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Application& operator=(Application&& other) = default;

      /**
       * @brief Perform initialisation operations for the application. Calls userInit_().
       */
      void init();

      /**
       * @brief Run the application.
       * @details The main loop does not use a rw::time::Timer because it must be executed by the main thread to support input events in
       * graphical applications.
       */
      void run();

      /**
       * @brief Set the duration of each main loop cycle.
       * @tparam T Type of data used for the duration.
       * @param duration Duration of the main loop in microseconds or any compatible std::chrono::duration.
       */
      template<rw::time::IsConvertibleToChronoDuration T>
      void setCycleDuration(T duration);

      /**
       * @brief Set the frequency of the main loop
       * @param hertz Main loop frequency in Hz.
       */
      void setCycleFrequency(f64 hertz);

      /**
       * @brief Perform a step of the application (one single execution of the main loop contained in run).
       */
      virtual void step();

      /**
       * @brief Stop the main loop, if it is running.
       */
      virtual void stop();

   protected:
      /**
       * @brief User initialisation function, called during init.
       */
      virtual void userInit_() = 0;

      /**
       * @brief User run function for custom operations that will be executed at each cycle by the main thread.
       */
      virtual void userRun_() = 0;

      /**
       * @brief Let the thread wait for the next main loop cycle.
       * @param remainingTime Time left until the next cycle begins.
       */
      void waitForNextCycle_(std::chrono::microseconds remainingTime);

      // Loop-related variables.
      std::atomic<bool>                          running_{ false };         /**< Loop control variable. If false, the main loop stops. */
      static constexpr std::chrono::microseconds cycle_control_value_{ 0 }; /**< Value to check against to verify if waiting is disabled.*/
      std::chrono::microseconds                  cycleDuration_{ 0 };       /**< Duration of each cycle of the main loop in microseconds. */

   private:
      std::vector<std::string> arguments_; /**< Command line arguments passed at the start of the application. */
   };
} // namespace rw::core

// IMPLEMENTATION
template<rw::time::IsConvertibleToChronoDuration T>
void rw::core::Application::setCycleDuration(T duration)
{
   if (duration >= cycle_control_value_)
   {
      cycleDuration_ = std::chrono::microseconds(duration);
   }
}

#endif