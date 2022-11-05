#include "EventTester.hpp"

EventTester::EventTester(rw::RedWolfManager& rw) : BaseObject(rw)
{
   timer_ = std::make_unique<rw::time::Timer>(rw, std::chrono::seconds(1));
   timer_->subscribe<rw::events::TimeoutEvent>(this);
   subscribe<EmptyTestEvent>(this);
}

void EventTester::runTest()
{
   timer_->start();

   while (currentIteration_ <= num_of_iterations)
   {
      generateEvent(EmptyTestEvent());
      ++eventsGenerated_;
   }
}

void EventTester::userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender)
{
   switch (evnt.id)
   {
   case rw::events::TimeoutEvent::event_id:
      logger_.relWarn(
         "Iteration {}, Events generated/dispatched: {}/{}.", currentIteration_.load(), eventsGenerated_.load(), eventsDispatched_.load());
      if (currentIteration_ >= num_of_iterations)
      {
         timer_->stop();
      }
      ++currentIteration_;
      eventsGenerated_ = 0U;
      eventsDispatched_ = 0U;
      break;
   case EmptyTestEvent::event_id:
      ++eventsDispatched_;
      break;
   }
}