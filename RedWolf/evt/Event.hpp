#ifndef RW_EVT_EVENT_HPP
#define RW_EVT_EVENT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/evt/aliases.hpp"

namespace rw::evt
{
   /**
    * @brief Base event class for all events.
    * @details All events must derive from this class. All derived classes must have an unsigned int member called event_id, which is a
    * unique identifier for the events that is used for event handling and propagation.
    */
   struct RW_API Event
   {
      /**
       * @brief List of reserved event IDs. These IDs MUST NOT be used in user events.
       */
      enum class ReservedId : EventID
      {
         invalid,
         button_press,
         mouse_movement,
         timeout,
         data_ready,
         window_close,
         draw,
         window_resize,
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
      static constexpr EventID max_reserved_id{ static_cast<EventID>(ReservedId::num_reserved) };

      /**
       * @brief Constructor.
       */
      explicit Event(EventID eventId);

      /**
       * @brief Virtual destructor.
       */
      virtual ~Event() = default;

      /**
       * @brief Copy constructor.
       */
      Event(const Event&) = default;

      /**
       * @brief Move constructor.
       */
      Event(Event&&) = default;

      /**
       * @brief Copy-assignment operator.
       */
      Event& operator=(const Event&) = default;

      /**
       * @brief Move-assignment operator.
       */
      Event& operator=(Event&&) = default;

      EventID id{ 0U }; /**< Event ID. */
   };
} // namespace rw::evt

#endif