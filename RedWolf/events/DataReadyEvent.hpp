#ifndef RW_EVENTS_DATAREADYEVENT_HPP
#define RW_EVENTS_DATAREADYEVENT_HPP

#include "RedWolf/events/BaseEvent.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace rw::events
{
   /**
    * @brief Event for signalling that a set of data is ready.
    */
   struct RW_API DataReadyEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::data_ready) }; /**< Event ID. */

      DataReadyEvent(const std::vector<std::byte>& sourceData, const std::string& sourceAddress, const std::string& sourcePort) :
         BaseEvent(event_id), data{ sourceData }, address{ sourceAddress }, port{ sourcePort } {};

      std::vector<std::byte> data;
      std::string            address;
      std::string            port;
   };
} // namespace rw::events

#endif