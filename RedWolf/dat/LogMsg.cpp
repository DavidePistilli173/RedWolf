#include "LogMsg.hpp"

namespace rw::dat
{
   LogMsg::LogMsg(Level lvl, std::source_location srcLoc, std::string_view msg) :
      level{ lvl }, dateTime{ std::chrono::system_clock::now() }, sourceLocation{ srcLoc }, message{ msg }
   {
   }
} // namespace rw::dat