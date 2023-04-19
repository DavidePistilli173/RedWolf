#include "RedWolfManager.hpp"

using namespace rw;

void RedWolfManager::destroyObject(rw::core::Object* obj)
{
   if (obj != nullptr) objects_.erase(obj->id());
}