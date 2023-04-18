#ifndef RW_CORE_OBJECT_HPP
#define RW_CORE_OBJECT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/aliases.hpp"

namespace rw::core
{
   /**
    * @brief Base class for all objects managed by RedWolf.
    */
   class RW_API Object
   {
   public:
      /**
       * @brief Constructor.
       */
      explicit Object(ObjectID id);

      /**
       * @brief Get the object's ID.
       * @return Object's ID.
       */
      [[nodiscard]] ObjectID id() const;

   private:
      ObjectID id_{ 0U }; /**< Object ID. */
   };
} // namespace rw::core

#endif