//
// Created by cjm036653 on 10/08/25.
//

#include "logger.hpp"

rw::util::Logger rw::util::Logger::instance_;

rw::util::Logger::Logger() {
    info("Logger created.");
}

rw::util::Logger& rw::util::Logger::get() {
    return instance_;
}

rw::util::Logger::Level rw::util::Logger::level() const {
    return level_;
}

void rw::util::Logger::set_level(Level level) {
    info("Loggger level changed from {} to {}.", level_.load(), level);
    level_ = level;
}
