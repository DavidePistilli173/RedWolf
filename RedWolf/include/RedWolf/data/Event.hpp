#ifndef RW_DATA_EVENT_HPP
#define RW_DATA_EVENT_HPP

#include "RedWolf/core.hpp"

#include <SDL2.0.9/SDL_events.h>

namespace rw::data
{
   struct RW_API Event
   {
      SDL_Event* evnt;
   };
} // namespace rw::data

#endif