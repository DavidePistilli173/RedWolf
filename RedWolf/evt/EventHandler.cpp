#include "EventHandler.hpp"

#include "RedWolf/alg/search.hpp"
#include "RedWolf/alg/sort.hpp"

using namespace rw::evt;

EventHandler::EventHandler(rw::util::Logger& logger) :
   logger_{ logger }, threadPool_{ std::max(1U, std::thread::hardware_concurrency() / 2) }
{
}

void EventHandler::addObject(rw::core::BaseObject* object)
{
   std::scoped_lock lck{ mtx_ };

   if (!objects_.contains(object))
   {
      objects_.emplace(object, std::unordered_map<EventID, std::vector<rw::core::BaseObject*>>());
   }
   else
   {
      logger_.warn("Object {} already added to the event system.", object);
   }
}

void EventHandler::removeObject(rw::core::BaseObject* object)
{
   std::scoped_lock lck{ mtx_ };

   if (objects_.contains(object))
   {
      objects_.erase(object);
   }

   // TODO: Optimise.
   for (auto& generator : objects_)
   {
      for (auto& eventMap : generator.second)
      {
         auto subscriberIterator = rw::alg::binarySearch(eventMap.second, object);
         if (subscriberIterator != eventMap.second.cend())
         {
            eventMap.second.erase(subscriberIterator);
         }
      }
   }
}

void EventHandler::subscribe(const rw::core::BaseObject* generator, rw::core::BaseObject* subscriber, EventID eventId)
{
   std::scoped_lock lck_{ mtx_ };
   if (!objects_.contains(generator))
   {
      logger_.warn("The event handler does not contain object: {}", generator);
      return;
   }
   if (!objects_.contains(subscriber))
   {
      logger_.warn("The event handler does not contain object: {}", subscriber);
      return;
   }

   if (rw::alg::binarySearch(objects_[generator][eventId], subscriber) == objects_[generator][eventId].cend())
   {
      objects_[generator][eventId].emplace_back(subscriber);
      rw::alg::sortLastElement(objects_[generator][eventId].begin(), objects_[generator][eventId].end());
   }
}

void EventHandler::unsubscribe(const rw::core::BaseObject* generator, rw::core::BaseObject* subscriber, EventID eventId)
{
   std::scoped_lock lck_{ mtx_ };
   if (!objects_.contains(generator))
   {
      return;
   }

   auto& eventMap = objects_[generator][eventId];
   auto  subscriberIterator = rw::alg::binarySearch(eventMap, subscriber);
   if (subscriberIterator != eventMap.cend())
   {
      eventMap.erase(subscriberIterator);
   }
}