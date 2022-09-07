#ifndef RW_CORE_BASEAPPLICATION_HPP
#define RW_CORE_BASEAPPLICATION_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/time/concepts.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <atomic>
#include <chrono>
#include <string>
#include <vector>

namespace rw::core
{
   /**
    * @brief Base class for all RedWolf applications. To be extended by users of the library.
    */
   class RW_API BaseApplication : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param argc Number of command line arguments.
       * @param argv List of command line arguments.
       */
      BaseApplication(int argc = 0, char** argv = nullptr);

      /**
       * @brief Destructor.
       */
      virtual ~BaseApplication() = default;

      /**
       * @brief Base applications cannot be copied.
       */
      BaseApplication(const BaseApplication&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseApplication(BaseApplication&& other) = default;

      /**
       * @brief Base applications cannot be copied
       */
      BaseApplication& operator=(const BaseApplication&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      BaseApplication& operator=(BaseApplication&& other) = default;

      /**
       * @brief Perform initialisation operations for the application. Calls userInit_().
       */
      virtual void init();

      /**
       * @brief Run the application.
       * @details The main loop does not use a rw::time::Timer because it must be executed by the main thread to support input events in
       * graphical applications.
       */
      virtual void run();

      /**
       * @brief Set the duration of each main loop cycle.
       * @tparam T Type of data used for the duration.
       * @param duration Duration of the main loop in microseconds or any compatible std::chrono::duration.
       */
      template<rw::time::IsConvertibleToChronoDuration T>
      void setCycleDuration(T duration)
      {
         if (duration >= cycle_control_value_)
         {
            cycleDuration_ = std::chrono::microseconds(duration);
            logger_->trace("Main loop duration set to {}.", cycleDuration_);
         }
         else
         {
            logger_->err("Main loop cycle duration must be a positive integer or 0, but {} was passed.", duration);
         }
      }

      /**
       * @brief Set the frequency of the main loop
       * @param hertz Main loop frequency in Hz.
       */
      void setCycleFrequency(double hertz);

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
       * @details To be implemented in derived classes.
       */
      virtual void userRun_() = 0;

      // Loop-related variables.
      std::atomic<bool>                          running_{ false };         /**< Loop control variable. If false, the main loop stops. */
      static constexpr std::chrono::microseconds cycle_control_value_{ 0 }; /**< Value to check against to verify if waiting is disabled.*/
      std::chrono::microseconds                  cycleDuration_{ 0 };       /**< Duration of each cycle of the main loop in microseconds. */

   private:
      std::vector<std::string> arguments_;         /**< Command line arguments passed at the start of the application. */
      rw::utils::Logger*       logger_{ nullptr }; /**< Logger. */
   };
} // namespace rw::core

#endif