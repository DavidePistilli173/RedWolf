#ifndef RW_EVENTS_BASEEVENT_HPP
#define RW_EVENTS_BASEEVENT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/events/aliases.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace rw::events
{
   /**
    * @brief Base event class for all events.
    * @detail All events must derive from this class. All derived classes must have an unsigned int member called event_id, which is a
    * unique identifier for the events that is used for event handling and propagation.
    */
   struct RW_API BaseEvent
   {
      /**
       * @brief List of reserved event IDs. These IDs MUST NOT be used in user events.
       */
      enum class ReservedEventId : EventID
      {
         invalid,
         button_press,
         mouse_movement,
         timeout,
         reserved_4,
         reserved_5,
         reserved_6,
         reserved_7,
         reserved_8,
         reserved_9,
         reserved_10,
         reserved_11,
         reserved_12,
         reserved_13,
         reserved_14,
         reserved_15,
         reserved_16,
         reserved_17,
         reserved_18,
         reserved_19,
         reserved_20,
         num_reserved
      };

      /**
       * @brief  Highest reserved event ID.
       */
      static constexpr EventID max_reserved_id{ static_cast<EventID>(ReservedEventId::num_reserved) };

      /**
       * @brief Constructor.
       */
      explicit BaseEvent(EventID eventId) : id{ eventId } {}

      /**
       * @brief Virtual destructor.
       */
      virtual ~BaseEvent() = default;

      EventID id{ 0U }; /**< Event ID. */
   };
} // namespace rw::events

#endif