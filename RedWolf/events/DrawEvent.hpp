#ifndef RW_EVENTS_DRAWEVENT_HPP
#define RW_EVENTS_DRAWEVENT_HPP

#include "RedWolf/events/BaseEvent.hpp"

namespace rw::events
{
   /**
    * @brief Event for signalling that draw calls are being made.
    */
   struct RW_API DrawEvent : public BaseEvent
   {
      static constexpr auto event_id{ static_cast<EventID>(BaseEvent::ReservedEventId::draw) }; /**< Event ID. */

      /**
       * @brief Default constructor.
       */
      DrawEvent() : BaseEvent(event_id){};

      /**
       * @brief Constructor.
       * @param sourceData Raw data produced.
       * @param sourceAddress Source address of the data.
       * @param sourcePort Source port of the data.
       */
      DrawEvent(size_t pPipelineId) : BaseEvent(event_id), pipelineId{ pPipelineId } {};

      size_t pipelineId{ 0U };
   };
} // namespace rw::events

#endif