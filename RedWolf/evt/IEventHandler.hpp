#ifndef RW_EVENT_IEVTHANDLER_HPP
#define RW_EVENT_IEVTHANDLER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/aliases.hpp"
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
       * @brief Generate an event.
       * @param sender ID of the event generator.
       * @param evt Event data.
       */
      void generateEvent(ObjectID sender, const Event& evt) const = 0;
   };
} // namespace rw::evt

#endif