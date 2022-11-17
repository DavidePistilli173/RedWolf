#include "EventHandler.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::events;

EventHandler::EventHandler(RedWolfManager& manager) : logger_{ manager.logger() }, threadPool_{ manager.threadPool() } {}

void EventHandler::addObject(rw::core::BaseObject* object)
{
   std::scoped_lock lck{ mtx_ };

   if (!objects_.contains(object))
   {
      objects_.emplace(object, std::map<EventID, std::vector<rw::core::BaseObject*>>());
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
}