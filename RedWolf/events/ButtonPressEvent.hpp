#ifndef RW_EVENTS_BUTTONPRESSEVENT_HPP
#define RW_EVENTS_BUTTONPRESSEVENT_HPP

#include "RedWolf/BaseEvent.hpp"
#include "RedWolf/common.hpp"

namespace rw::events
{
   struct RW_API ButtonPressEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::button_press) };

      explicit ButtonPressEvent((ButtonPressEvent*) nullptr) : BaseEvent(event_id) {}

      i32 btnId{ 0 };
   };
} // namespace rw::events

#endif