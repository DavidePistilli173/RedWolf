//
// Created by cjm036653 on 10/08/25.
//

#include "log_msg.hpp"

rw::util::LogMsg::LogMsg(Level lvl, std::source_location src_loc, std::string_view msg) :
    level{ lvl }, date_time{ std::chrono::system_clock::now() }, source_location{ src_loc }, message{ msg } {}