#ifndef RW_CORE_OBJECT_HPP
#define RW_CORE_OBJECT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/core/aliases.hpp"
#include "RedWolf/evt/EventHandler.hpp"

namespace rw::core
{
   /**
    * @brief Base class for all objects managed by RedWolf.
    */
   class RW_API Object : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       */
      explicit Object() = default;

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
      template<IsDerivedFrom<rw::evt::Event> T>
      void generateEvent(const T& evt);

      /**
       * @brief Set the event handler that will dispatch events from the object.
       * @param eventHandler Event handler to set.
       */
      void setEventHandler(rw::evt::EventHandler* eventHandler);

   protected:
      /**
       * @brief Function to override in child classes to add custom event handling.
       * @param generator Event generator. Will always be nullptr.
       * @param evt Event data.
       */
      virtual void userHandle_(const BaseObject* generator, const rw::evt::Event& evt) = 0;

   private:
      rw::evt::EventHandler* eventHandler_{ nullptr }; /**< Event handler that will dispatch the events from the object. */
   };
} // namespace rw::core

// IMPLEMENTATION
template<rw::core::IsDerivedFrom<rw::evt::Event> T>
void rw::core::Object::generateEvent(const T& evt)
{
   if (eventHandler_ != nullptr)
   {
      eventHandler_->generateEvent(this, evt);
   }
}

#endif