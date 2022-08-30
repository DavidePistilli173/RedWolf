#ifndef RWSANDBOX_MAINAPPLICATION_HPP
#define RWSANDBOX_MAINAPPLICATION_HPP

#include "RedWolf/core/BaseApplication.hpp"
#include "RedWolf/time/Timer.hpp"
#include "RedWolf/utils/Logger.hpp"

/**
 * @brief Main application of the RedWolfSandbox program.
 */
class MainApplication : public rw::core::BaseApplication
{
public:
   /**
    * @brief Constructor.
    * @param argc Number of input arguments.
    * @param argv Input arguments.
    */
   explicit MainApplication(int argc, char** argv);

protected:
   /**
    * @brief User handle function for incoming events.
    * @param evnt Event data.
    * @param sender Sender of the event.
    */
   virtual void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) override;

   /**
    * @brief User initialisation function.
    */
   virtual void userInit_() override;

   /**
    * @brief User run function for custom operations that will be executed at each cycle by the main thread.
    * @details To be implemented in derived classes.
    */
   virtual void userRun_() override;

private:
   rw::utils::Logger* logger_{ nullptr }; /**< Application logger. */

   size_t mainLoopIteration_{ 0U }; /**< Number of iterations of the main loop. */
   size_t totalIterations_{ 0U };   /**< Total number of iterations of the main loop. */

   rw::time::Timer     timer_;            /**< Timer. */
   std::atomic<size_t> timerCount_{ 0U }; /**< Number of times the timer was called since the last main loop iteration. */
};

#endif