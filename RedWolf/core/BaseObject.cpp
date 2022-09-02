#include "BaseObject.hpp"

namespace rw::core
{
   BaseObject::BaseObject(BaseObject* parent) : parent_{ parent }, threadPool_{ rw::thread::ThreadPool::defaultPool() } {}

   void BaseObject::setParent(BaseObject* parent)
   {
      parent_ = parent;
   }
} // namespace rw::core