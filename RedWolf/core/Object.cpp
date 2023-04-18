#include "Object.hpp"

using namespace rw::core;

Object::Object(ObjectID id) : id_{ id } {};

ObjectID Object::id() const
{
   return id_;
}