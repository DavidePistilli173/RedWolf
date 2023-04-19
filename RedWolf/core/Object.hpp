#ifndef RW_CORE_OBJECT_HPP
#define RW_CORE_OBJECT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/IObject.hpp"
#include "RedWolf/core/aliases.hpp"
#include "RedWolf/evt/IEventHandler.hpp"

namespace rw::core
{
   /**
    * @brief Base class for all objects managed by RedWolf.
    */
   class RW_API Object : public IObject
   {
   public:
      /**
       * @brief Constructor.
       */
      explicit Object(ObjectID id);

      /**
       * @brief Destructor.
       */
      virtual ~Object() = default;

      /**
       * @brief Copy constructor.
       */
      Object(const Object&) = delete;

      /**
       * @brief Move constructor.
       */
      Object(Object&&) = default;

      /**
       * @brief Copy-assignment operator.
       */
      Object& operator=(const Object&) = delete;

      /**
       * @brief Move-assignment oeprator.
       */
      Object& operator=(Object&&) = default;

      /**
       * @brief Generate an event.
       * @param evt Event data.
       */
      void generateEvent(const rw::evt::Event& evt) const;

      /**
       * @brief Get the object's ID.
       * @return Object's ID.
       */
      [[nodiscard]] ObjectID id() const;

      /**
       * @brief Set the event handler that will dispatch events from the object.
       * @param eventHandler Event handler to set.
       */
      void setEventHandler(const rw::evt::IEventHandler* eventHandler);

   private:
      ObjectID                      id_{ 0U };                /**< Object ID. */
      const rw::evt::IEventHandler* eventHandler_{ nullptr }; /**< Event handler that will dispatch the events from the object. */
   };
} // namespace rw::core

#endif