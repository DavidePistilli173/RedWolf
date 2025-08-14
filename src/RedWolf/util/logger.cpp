//
// Created by cjm036653 on 10/08/25.
//

#include "logger.hpp"

rw::util::Logger& rw::util::Logger::get() {
    static Logger instance;
    return instance;
}

rw::util::Logger::Level rw::util::Logger::level() const {
    return level_;
}

void rw::util::Logger::set_level(Level level) {
    info("Logger level changed from {} to {}.", level_.load(), level);
    level_ = level;
}

rw::util::Logger::Logger() {
    info("Logger created.");
}