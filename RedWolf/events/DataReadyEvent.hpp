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

      /**
       * @brief Default constructor.
       */
      DataReadyEvent() : BaseEvent(event_id){};

      /**
       * @brief Constructor.
       * @param sourceData Raw data produced.
       * @param sourceAddress Source address of the data.
       * @param sourcePort Source port of the data.
       */
      DataReadyEvent(const std::vector<std::byte>& sourceData, const std::string& sourceAddress, const std::string& sourcePort) :
         BaseEvent(event_id), data{ sourceData }, address{ sourceAddress }, port{ sourcePort } {};

      std::vector<std::byte> data;    /**< Raw data. */
      std::string            address; /**< Address of the data source. */
      std::string            port;    /**< Port of the data source. */
   };
} // namespace rw::events

#endif