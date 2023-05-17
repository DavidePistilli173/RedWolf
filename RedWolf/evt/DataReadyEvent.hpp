#ifndef RW_EVENTS_DATAREADYEVENT_HPP
#define RW_EVENTS_DATAREADYEVENT_HPP

#include "RedWolf/evt/Event.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace rw::evt
{
   /**
    * @brief Event for signalling that a set of data is ready.
    */
   struct RW_API DataReadyEvent : public Event
   {
      static constexpr auto event_id{ static_cast<EventID>(Event::ReservedId::data_ready) }; /**< Event ID. */

      /**
       * @brief Default constructor.
       */
      DataReadyEvent() : Event(event_id){};

      /**
       * @brief Constructor.
       * @param sourceData Raw data produced.
       * @param sourceAddress Source address of the data.
       * @param sourcePort Source port of the data.
       */
      DataReadyEvent(const std::vector<std::byte>& sourceData, const std::string& sourceAddress, const std::string& sourcePort) :
         Event(event_id), data{ sourceData }, address{ sourceAddress }, port{ sourcePort } {};

      std::vector<std::byte> data;    /**< Raw data. */
      std::string            address; /**< Address of the data source. */
      std::string            port;    /**< Port of the data source. */
   };
} // namespace rw::evt

#endif