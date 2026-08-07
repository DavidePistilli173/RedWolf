#include "logger.hpp"

namespace {
    rw::Logger* g_logger{ nullptr };
}

void rw::Logger::init() {
#ifdef RW_ENABLE_LOGGING
    if (nullptr != g_logger) {
        return;
    }

    g_logger = new Logger();
    trace("Logger created");
#else
    // No instance is created since logging is disabled.
#endif
}

rw::Logger* rw::Logger::instance() {
    return g_logger;
}

void rw::Logger::shutdown() {
    if (nullptr != g_logger) {
        delete g_logger;
        g_logger = nullptr;
    }
}