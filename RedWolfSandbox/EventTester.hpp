#ifndef RWSANDBOX_EVENTTESTER_HPP
#define RWSANDBOX_EVENTTESTER_HPP

#include <RedWOlf/time/Timer.hpp>
#include <RedWolf/core/BaseObject.hpp>
#include <atomic>
#include <memory>

/**
 * @brief Empty test event.
 */
struct EmptyTestEvent : public rw::events::BaseEvent
{
   static constexpr rw::events::EventID event_id{ 1000U }; /**< Event ID. */

   EmptyTestEvent() : BaseEvent(event_id) {}
};

/**
 * @brief Class for testing the speed of the event system.
 */
class EventTester : public rw::core::BaseObject
{
public:
   static constexpr size_t num_of_iterations{ 10U }; /**< Number of timer iterations to run. */

   /**
    * @brief Constructor.
    * @param rw RedWolf manager.
    */
   EventTester(rw::RedWolfManager& rw);

   /**
    * @brief Run the test.
    */
   void runTest();

protected:
   /**
    * @brief User handle function for incoming events.
    * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because
    * events could be generated from different threads (eg. rw::time::Timer).
    * @param evnt Event data.
    * @param sender Sender of the event.
    */
   virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender);

private:
   std::atomic<size_t> eventsGenerated_{ 0U };  /**< Number of events generated. */
   std::atomic<size_t> eventsDispatched_{ 0U }; /**< Number of events delivered. */

   std::unique_ptr<rw::time::Timer> timer_;                  /**< Timer to check and log the results. */
   std::atomic<size_t>              currentIteration_{ 1U }; /**< Current iteration. */
};

#endif