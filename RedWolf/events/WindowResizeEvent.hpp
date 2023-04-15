#ifndef RW_EVENTS_WINDOWRESIZEEVENT_HPP
#define RW_EVENTS_WINDOWRESIZEEVENT_HPP

#include "RedWolf/events/BaseEvent.hpp"

namespace rw::events
{
   /**
    * @brief Event for signalling a timer's timeout.
    */
   struct RW_API WindowResizeEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::window_resize) }; /**< Event ID. */

      WindowResizeEvent(i32 w, i32 h) : BaseEvent(event_id), width(w), height(h){};

      i32 width{ 0 };  /**< New width of the window. */
      i32 height{ 0 }; /**< New height of the window. */
   };
} // namespace rw::events

#endif