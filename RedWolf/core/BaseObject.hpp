#ifndef RW_CORE_BASEOBJECT_HPP
#define RW_CORE_BASEOBJECT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/events/BaseEvent.hpp"
#include "RedWolf/events/EventHandler.hpp"
#include "RedWolf/events/concepts.hpp"

#include <iostream>
#include <map>
#include <set>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <utility>

namespace rw
{
   class RedWolfManager;
}

namespace rw::core
{
   /**
    * @brief Base class for all objects that need to be hooked up with the event system of RedWolf.
    */
   class RW_API BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param parent Logical parent of the object. Not related to memory management like in other libraries (eg. QT).
       */
      explicit BaseObject(RedWolfManager& manager, BaseObject* parent = nullptr);

      /**
       * @brief Destructor.
       */
      ~BaseObject();

      /**
       * @brief Copy constructor.
       */
      BaseObject(const BaseObject&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseObject(BaseObject&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      BaseObject& operator=(const BaseObject&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      BaseObject& operator=(BaseObject&&) = delete;

      /**
       * @brief Generate an event and propagate it to all subscribers using a dedicated thread.
       * @tparam Event Event type to generate.
       * @param evnt Event data to send.
       */
      template<rw::events::HasEventID Event>
      void generateEvent(const Event& evnt)
      {
         eventHandler_.generateEvent<Event>(this, evnt);
      }

      /**
       * @brief Handle an incoming event.
       * @param evnt Event data.
       * @param sender Sender of the event.
       */
      virtual void handle(const rw::events::BaseEvent& evnt, const BaseObject* sender);

      /**
       * @brief Set a new parent for the object.
       * @detail This function does not change the parent subscriptions from this object. For this, each derived class must override this
       * method and add specific functionality.
       * @param parent New parent.
       */
      virtual void setParent(BaseObject* parent);

      /**
       * @brief Subscribe an object to an event generated by this object.
       * @tparam Event Event type that the object wants to receive.
       * @param object Object that wants to subscribe to the event.
       */
      template<rw::events::HasEventID Event>
      void subscribe(BaseObject* object)
      {
         eventHandler_.subscribe<Event>(this, object);
      }

      /**
       * @brief Unsubscribe a previously subscribed object from a specific event.
       * @tparam Event Event type that the object wants to unsubscribe from.
       * @param object Object that wants to unsubscribe from the event.
       */
      template<rw::events::HasEventID Event>
      void unsubscribe(BaseObject* object)
      {
         eventHandler_.unsubscribe<Event>(this, object);
      }

   protected:
      /**
       * @brief User handle function for incoming events. To be implemented in derived classes.
       * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because
       * events could be generated from different threads (eg. rw::time::Timer).
       * @param evnt Event data.
       * @param sender Sender of the event.
       */
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) = 0;

   private:
      BaseObject* parent_{ nullptr }; /**< Logical parent of the object. */

      rw::events::EventHandler& eventHandler_; /**< RedWolf event system handler. */
   };
} // namespace rw::core

#endif