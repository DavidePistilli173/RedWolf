#include "Object.hpp"

using namespace rw::core;

Object::Object(ObjectID id) : id_{ id } {};

void Object::generateEvent(const rw::evt::Event& evt) const
{
   if (eventHandler_ != nullptr)
   {
      eventHandler_->generateEvent(id_, evt);
   }
}

ObjectID Object::id() const
{
   return id_;
}

void Object::setEventHandler(const rw::evt::IEventHandler* eventHandler)
{
   eventHandler_ = eventHandler;
}