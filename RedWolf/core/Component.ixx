module;

#include "RedWolf/common.hpp"

export module rw.core.Component;

import rw.core.aliases;

export namespace rw::core
{
   struct RW_API Component
   {
      constexpr Component(ComponentID pId);

      const ComponentID id;
   };
} // namespace rw::core

// IMPLEMENTATION
using namespace rw::core;

constexpr Component::Component(ComponentID pId) : id{ pId } {}