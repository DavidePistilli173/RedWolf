#include "BaseObject.hpp"

namespace rw::core
{
   BaseObject::BaseObject(RedWolfManager& manager, BaseObject* parent) : parent_{ parent }, threadPool_{ manager.threadPool() } {}

   void BaseObject::handle(const rw::events::BaseEvent& evnt, const BaseObject* sender)
   {
      userHandle_(evnt, sender);
   }

   void BaseObject::setParent(BaseObject* parent)
   {
      parent_ = parent;
   }
} // namespace rw::core