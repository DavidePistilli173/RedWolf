#ifndef RW_EVENTS_TIMEOUTEVENT_HPP
#define RW_EVENTS_TIMEOUTEVENT_HPP

#include "RedWolf/events/BaseEvent.hpp"

namespace rw::events
{
   /**
    * @brief Event for signalling a timer's timeout.
    */
   struct RW_API TimeoutEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::timeout) }; /**< Event ID. */

      TimeoutEvent() : BaseEvent(event_id){};
   };
} // namespace rw::events

#endif