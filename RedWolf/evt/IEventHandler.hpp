#ifndef RW_EVENT_IEVTHANDLER_HPP
#define RW_EVENT_IEVTHANDLER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/IObject.hpp"
#include "RedWolf/core/concepts.hpp"
#include "RedWolf/evt/Event.hpp"

namespace rw::evt
{
   /**
    * @brief Event handler interface, used by RedWolf objects.
    */
   class RW_API IEventHandler
   {
   public:
      /**
       * @brief All supported policies for generating events.
       */
      enum class GenerationPolicy
      {
         asynchronous, /**< Event generation is performed asynchronously on another thread. */
         synchronous   /**< Event generation is performed synchronously on the calling thread. */
      };

      /**
       * @brief Generate an event.
       * @param generator Event generator.
       * @param evt Event data.
       * @param policy Policy for the event generation.
       */
      virtual void
         generateEvent(rw::core::IObject* generator, const Event& evt, GenerationPolicy policy = GenerationPolicy::asynchronous) const = 0;
   };
} // namespace rw::evt

#endif