#include "RedWolfManager.hpp"

using namespace rw;

void RedWolfManager::destroyObject(rw::core::Object* obj)
{
   objects_.erase(obj);
}