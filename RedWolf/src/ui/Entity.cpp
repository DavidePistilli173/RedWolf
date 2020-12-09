#include "RedWolf/ui/Entity.hpp"

namespace rw::ui
{
   /** 
      \param id Specific ID to assign to the entity. 
   */
   Entity::Entity(size_t id) :
      id_{ id }
   {
   }

   size_t Entity::id() const
   {
      return id_;
   }
} // namespace rw::ui