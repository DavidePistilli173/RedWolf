module;

#include "RedWolf/common.hpp"

export module rw.core.EntityDB;

import rw.core.aliases;
import rw.core.concepts;
import rw.core.Component;

import <memory>;
import <mutex>;
import <ranges>;
import <unordered_map>;
import <utility>;
import <vector>;

export namespace rw::core
{
   /**
    @brief Entity-component database.
   */
   class RW_API EntityDB
   {
   public:
      /**
       * @brief Construct an empty entity database.
       */
      EntityDB() = default;

      /**
       * @brief Destructor.
       */
      ~EntityDB() = default;

      /**
       * @brief Copy constructor.
       */
      EntityDB(EntityDB&) = delete;

      /**
       * @brief Move constructor.
       */
      EntityDB(EntityDB&&) = default;

      /**
       @brief Copy-assignment operator.
      */
      EntityDB& operator=(EntityDB&) = delete;

      /**
       @brief Move-assignment operator.
      */
      EntityDB& operator=(EntityDB&&) = default;

      template<IsDerivedFrom<Component>... Comps>
      [[nodiscard]] auto getEntitiesWithComponents();

   private:
      size_t            firstFreeEntityId_{ 0U }; /**< Next entity ID that will be allocated. */
      std::vector<bool> entityIds_;               /**< Active entity IDs. */

      std::unordered_map<ComponentID, std::vector<std::pair<EntityID, std::unique_ptr<Component>>>> db_; /**< Entity-component DB. */
   };
} // namespace rw::core

// IMPLEMENTATION
using namespace rw::core;

template<IsDerivedFrom<Component>... Comps>
[[nodiscard]] auto EntityDB::getEntitiesWithComponents()
{
   // std::vector<std::vector<std::pair<EntityID, std::unique_ptr<Component>>>&> componentArrays = { ((db_[Comps::typeId()]), ...) };

   auto v = std::vector{ ((db_[Comps::typeId()]), ...) };
}
