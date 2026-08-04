#include "redwolf/logger.hpp"

namespace {
    rw::Logger* g_instance{ nullptr };
}

void rw::Logger::init() {
    if (nullptr != instance) {
        return;
    }

    g_instance = new Logger();
    trace("Logger created");
}

rw::Logger* rw::Logger::instance() {
    return g_instance;
}

rw::Logger::Logger() {}