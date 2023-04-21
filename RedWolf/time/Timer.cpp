#include "Timer.hpp"

#include "RedWolf/time/constants.hpp"

using namespace rw::time;

Timer::Timer() : rw::core::Object() {}

Timer::~Timer()
{
   stop();
   if (timerThread_.joinable())
   {
      timerThread_.join();
   }
}

f64 Timer::frequency() const
{
   return microseconds_per_second / interval_.count();
}

std::chrono::microseconds Timer::interval() const
{
   return interval_;
}

void Timer::pause()
{
   if (running())
   {
      std::scoped_lock lck(mtx_);
      paused_ = true;
   }
   stop();
}

bool Timer::paused() const
{
   return paused_;
}

std::chrono::microseconds Timer::remainingTime() const
{
   return remainingTime_ -
          std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - lastTimerStart_);
}

void Timer::restart()
{
   stop();
   start();
}

void Timer::resume()
{
   if (paused())
   {
      std::scoped_lock lck(mtx_);
      running_ = true;
      paused_ = false;
      createTimerThread_();
   }
}

bool Timer::running() const
{
   return (running_ && timerThread_.joinable());
}

void Timer::setFrequency(f64 hertz)
{
   if (hertz >= 0.0F)
   {
      std::scoped_lock lck(mtx_);
      interval_ = std::chrono::microseconds(static_cast<i64>(microseconds_per_second / hertz));
      logger_.trace("Timer frequency set to {}Hz (interval_ = {})", hertz, interval_);
   }
   else
   {
      logger_.err("Timer frequency must be a positive double or 0, but {} was passed.", hertz);
   }
}

void Timer::start()
{
   // If the thread is already running, return.
   if (running()) return;

   // Prepare the timer initial variables.
   std::scoped_lock lck(mtx_);
   running_ = true;
   paused_ = false;
   remainingTime_ = interval_;

   createTimerThread_();
}

void Timer::stop()
{
   if (running())
   {
      std::scoped_lock lck(mtx_);
      running_ = false;
   }
   timerConditionVariable_.notify_one();
}

void Timer::userHandle_(const rw::events::BaseEvent&, const BaseObject*) {}

void Timer::createTimerThread_()
{
   // Create the timer thread.
   timerThread_ = std::thread(
      [this]()
      {
         while (running_)
         {
            if (interval_ < busy_waiting_threshold)
            {
               lastTimerStart_ = std::chrono::high_resolution_clock::now();

               while (interval_ - std::chrono::duration_cast<std::chrono::microseconds>(
                                     std::chrono::high_resolution_clock::now() - lastTimerStart_) >
                      std::chrono::microseconds(0))
               {
               }
               generateEvent(rw::events::TimeoutEvent());
               if (singleShot_) running_ = false;
            }
            else
            {
               std::unique_lock lck(mtx_);
               // Sleep for the remaining time.
               lastTimerStart_ = std::chrono::high_resolution_clock::now();

               if (timerConditionVariable_.wait_for(lck, remainingTime_) == std::cv_status::timeout)
               {
                  // If the sleep reached timeout, generate the timeout event.
                  generateEvent(rw::events::TimeoutEvent());
                  remainingTime_ = interval_;
                  if (singleShot_) running_ = false;
               }
               else
               {
                  // If an interrupt was performed, recompute the remaining time.
                  remainingTime_ = interval_ - std::chrono::duration_cast<std::chrono::microseconds>(
                                                  std::chrono::high_resolution_clock::now() - lastTimerStart_);
               }
            }
         }
      });
}