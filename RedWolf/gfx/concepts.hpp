#ifndef RW_GFX_CONCEPTS_HPP
#define RW_GFX_CONCEPTS_HPP

#include "RedWolf/gfx/BaseEntity.hpp"

namespace rw::gfx
{
   template<typename T>
   concept IsDerivedFromBaseEntity = std::is_base_of_v<BaseEntity, T>;
}

#endif