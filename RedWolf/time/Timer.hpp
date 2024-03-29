#ifndef RW_TIME_TIMER_HPP
#define RW_TIME_TIMER_HPP

#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/events/TimeoutEvent.hpp"
#include "RedWolf/time/concepts.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace rw::time
{
   /**
    * @brief Configurable timer that will generate an event upon timeout. Note that event handling will happen in the timer's thread (that
    *        is different from the main thread).
    * @details The actual timer precision can vary depending on the scheduling performed by the operating system.
    *          For timers with an interval lower than busy_waiting_threshold, in order to get the required precision a busy-waiting approach
    *          was used, so use these high frequency timers with caution.
    */
   class Timer : public rw::core::BaseObject
   {
   public:
      /**
       * @brief Lower intervals will be dealt with through busy waiting.
       */
      static constexpr std::chrono::microseconds busy_waiting_threshold{ 50000 };

      /**
       * @brief Default constructor.
       */
      explicit Timer(rw::core::BaseObject* parent = nullptr);

      /**
       * @brief Construct a timer with a given interval and a singleShot setting.
       * @tparam T Data type used for the time interval.
       * @param interval Time interval in microseconds or any compatible std::chrono::duration.
       * @param singleShot If true, the timer will run only once for each call to start.
       */
      template<IsConvertibleToChronoDuration T>
      explicit Timer(T interval, bool singleShot = false, rw::core::BaseObject* parent = nullptr) :
         rw::core::BaseObject(parent), logger_{ rw::utils::Logger::instance() }, interval_{ std::chrono::microseconds(interval) },
         singleShot_{ singleShot }
      {
      }

      /**
       * @brief The destructor stops the timer, stopping any threads that are still running.
       */
      ~Timer();

      /**
       * @brief Copy constructor.
       */
      Timer(const Timer&) = delete;

      /**
       * @brief Default constructor.
       */
      Timer(Timer&&) = default;

      /**
       * @brief Copy assignment operator.
       * @return Reference to the new timer.
       */
      Timer& operator=(const Timer&) = delete;

      /**
       * @brief Move assignment operator.
       * @return Reference to the new timer.
       */
      Timer& operator=(Timer&&) = default;

      /**
       * @brief Get the current frequency of the timer.
       * @return Frequency in Hz.
       */
      [[nodiscard]] double frequency() const;

      /**
       * @brief Get the current interval of the timer.
       * @return Interval of the timer in microseconds.
       */
      [[nodiscard]] std::chrono::microseconds interval() const;

      /**
       * @brief Pause the timer.
       */
      void pause();

      /**
       * @brief Check if the timer is paused.
       * @return true The timer is paused.
       * @return false The timer is not paused.
       */
      [[nodiscard]] bool paused() const;

      /**
       * @brief Get the currently remaining time until the next timeout.
       * @return Remaining time in microseconds.
       */
      [[nodiscard]] std::chrono::microseconds remainingTime() const;

      /**
       * @brief If the timer has already started, restart it. Otherwise, start it.
       */
      void restart();

      /**
       * @brief If the timer was paused, resume it. Otherwise do nothing.
       */
      void resume();

      /**
       * @brief Check if the timer is running.
       * @return true The timer is running.
       * @return false The timer is not running.
       */
      [[nodiscard]] bool running() const;

      /**
       * @brief Set the frequency of the main loop
       * @param hertz Main loop frequency in Hz.
       */
      void setFrequency(double hertz);

      /**
       * @brief Set the timer interval.
       * @tparam T Type of data passed as interval.
       * @param interval Interval in microseconds (or compatible std::chrono::duration).
       */
      template<IsConvertibleToChronoDuration T>
      void setInterval(T interval)
      {
         std::scoped_lock lck(mtx_);
         interval_ = std::chrono::microseconds(interval);
      }

      /**
       * @brief Start the timer.
       */
      void start();

      /**
       * @brief Set a new interval and start the timer.
       * @tparam T Type of data passed as interval.
       * @param interval Interval in microseconds (or compatible std::chrono::duration).
       */
      template<IsConvertibleToChronoDuration T>
      void start(T interval)
      {
         setInterval(interval);
         start();
      }

      /**
       * @brief Stop the timer.
       */
      void stop();

   protected:
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender) override;

   private:
      /**
       * @brief Create the timer thread with the settings currently stored in the timer.
       */
      void createTimerThread_();

      rw::utils::Logger* logger_; /**< Message logger. */

      bool                      singleShot_{ false }; /**< If true, the timer only runs once. */
      std::chrono::microseconds interval_{ 0 };       /**< Timer interval. */

      bool                                  running_{ false };   /**< If true, the timer is running, otherwise it is false. */
      bool                                  paused_{ false };    /**< If true, the timer is simply paused. */
      std::chrono::steady_clock::time_point lastTimerStart_;     /**< Last time the timer started. */
      std::chrono::microseconds             remainingTime_{ 0 }; /**< Remaining sleep time. */

      std::thread             timerThread_;            /**< Thread that handles the timer. */
      std::mutex              mtx_;                    /**< Mutex for accessing timer variables. */
      std::condition_variable timerConditionVariable_; /**< Condition variable for the timer. */
   };
} // namespace rw::time

#endif