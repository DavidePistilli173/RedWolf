#include "EventHandler.hpp"

using namespace rw::evt;

EventHandler::EventHandler(rw::util::Logger& logger) :
   logger_{ logger }, threadPool_{ std::max(1U, std::thread::hardware_concurrency() / 2) }
{
}

void EventHandler::addObject(rw::core::Object* object)
{
   std::scoped_lock lck{ mtx_ };

   if (!objects_.contains(object))
   {
      objects_.emplace(object, std::unordered_map<EventID, std::vector<rw::core::Object*>>());
   }
   else
   {
      logger_.warn("Object {} already added to the event system.", object);
   }
}

void EventHandler::generateEvent(
   rw::core::IObject* generator, const Event& evt, GenerationPolicy policy = GenerationPolicy::asynchronous) const
{
   auto propagateEvent = [this, generator, evt]()
   {
      std::shared_lock lck_{ mtx_ };

      if (!objects_.contains(generator))
      {
         logger_.warn("Event generator {} already destroyed.", generator);
         return;
      }

      for (auto& subscriber : objects_[generator][evt.id])
      {
         subscriber->handle(evt, generator);
      }
   };

   switch (policy)
   {
   case GenerationPolicy::asynchronous:
      threadPool_.addTask(propagateEvent);
      break;
   case GenerationPolicy::synchronous:
      propagateEvent();
      break;
   }
}

void EventHandler::removeObject(rw::core::BaseObject* object)
{
   std::scoped_lock lck{ mtx_ };

   if (objects_.contains(object))
   {
      objects_.erase(object);
   }
}