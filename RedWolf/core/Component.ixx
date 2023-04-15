module;

#include "RedWolf/common.hpp"

export module rw.core.Component;

import rw.core.aliases;

import <atomic>;

export namespace rw::core
{
   /**
    * @brief Base component class.
    */
   struct RW_API Component
   {
      /**
       * @brief Constructor.
       * @param pId Component type ID. Should be unique.
       */
      constexpr Component(ComponentID pId);

      /**
       * @brief Get the type ID for a given component.
       * @tparam T Type of component to get the type ID of.
       * @return ID of the given component type.
       */
      template<typename T>
      [[nodiscard]] static ComponentID typeId();

      const ComponentID id; /**< Type ID. */

   private:
      static std::atomic<ComponentID> typeIdSeq_; /**< Next type ID that will be allocated. */
   };
} // namespace rw::core

// IMPLEMENTATION
using namespace rw::core;

constexpr Component::Component(ComponentID pId) : id{ pId } {}

template<typename T>
ComponentID Component::typeId()
{
   static ComponentID id{ typeIdSeq_++ };
   return id;
}

std::atomic<ComponentID> Component::typeIdSeq_{ 0U };