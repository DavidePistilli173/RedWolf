#include "RedWolfManager.hpp"

using namespace rw;

RedWolfManager::RedWolfManager(const Options& options) : options_{ options }, eventHandler_{ logger_ } {}

void RedWolfManager::destroyObject(rw::core::Object* obj)
{
   if (obj != nullptr)
   {
      std::scoped_lock lck{ mtx_ };

      eventHandler_.removeObject(obj);
      objects_.erase(obj);
   }
}

void RedWolfManager::subscribe(const rw::core::BaseObject* generator, rw::core::BaseObject* subscriber, rw::evt::EventID eventId)
{
   eventHandler_.subscribe(generator, subscriber, eventId);
}

void RedWolfManager::unsubscribe(const rw::core::BaseObject* generator, rw::core::BaseObject* subscriber, rw::evt::EventID eventId)
{
   eventHandler_.unsubscribe(generator, subscriber, eventId);
}