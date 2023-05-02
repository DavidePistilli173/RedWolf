#ifndef RW_EVT_TIMEOUTEVENT_HPP
#define RW_EVT_TIMEOUTEVENT_HPP

#include "RedWolf/evt/Event.hpp"

namespace rw::evt
{
   /**
    * @brief Event for signalling a timer's timeout.
    */
   struct RW_API TimeoutEvent : public Event
   {
      static constexpr auto event_id{ static_cast<EventID>(Event::ReservedId::timeout) }; /**< Event ID. */

      TimeoutEvent() : Event(event_id){};
   };
} // namespace rw::evt

#endif