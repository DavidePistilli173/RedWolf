#include "BaseObject.hpp"

namespace rw::core
{
   BaseObject::BaseObject(BaseObject* parent) : parent_{ parent }, threadPool_{ rw::thread::ThreadPool::defaultPool() } {}
} // namespace rw::core