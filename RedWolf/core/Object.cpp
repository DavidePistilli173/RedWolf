#include "Object.hpp"

using namespace rw::core;

void Object::setEventHandler(rw::evt::EventHandler* eventHandler)
{
   eventHandler_ = eventHandler;
}