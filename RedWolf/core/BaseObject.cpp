#include "BaseObject.hpp"

using namespace rw::core;

void BaseObject::handle(const BaseObject* generator, const rw::evt::Event& evt)
{
   if (generator != nullptr)
   {
      userHandle_(generator, evt);
   }
}