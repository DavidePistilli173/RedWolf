#ifndef RW_EVENTS_CONCEPTS_HPP
#define RW_EVENTS_CONCEPTS_HPP

#include "RedWolf/events/aliases.hpp"

#include <type_traits>

namespace rw::events
{
   /**
    * @brief Concept for types that have an event ID.
    */
   template<typename Event>
   concept HasEventID =
      std::is_same<decltype(Event::event_id), EventID>::value || std::is_same<decltype(Event::event_id), const EventID>::value;
} // namespace rw::events

#endif