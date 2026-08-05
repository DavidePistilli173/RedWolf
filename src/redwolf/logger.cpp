#include "redwolf/logger.hpp"

namespace {
    rw::Logger* g_instance{ nullptr };
}

void rw::Logger::init() {
#ifdef RW_ENABLE_LOGS
    if (nullptr != g_instance) {
        return;
    }

    g_instance = new Logger();
    trace("Logger created");
#else
#endif
}

rw::Logger* rw::Logger::instance() {
    return g_instance;
}

rw::Logger::Logger() {}