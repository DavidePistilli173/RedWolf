#include "LogMsg.hpp"

namespace rw::dat
{
   LogMsg::LogMsg(Level lvl, std::string_view msg) : level{ lvl }, dateTime{ std::chrono::system_clock::now() }, message{ msg } {}
} // namespace rw::dat