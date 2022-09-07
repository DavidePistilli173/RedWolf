#ifndef RW_EVENTS_WINDOWCLOSEEVENT_HPP
#define RW_EVENTS_WINDOWCLOSEEVENT_HPP

#include "RedWolf/events/BaseEvent.hpp"

namespace rw::events
{
   /**
    * @brief Event for signalling a timer's timeout.
    */
   struct RW_API WindowCloseEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::window_close) }; /**< Event ID. */

      WindowCloseEvent() : BaseEvent(event_id){};
   };
} // namespace rw::events

#endif