#include "events.hpp"

#include "redwolf/logger.hpp"

namespace {
    rw::Events* g_events{ nullptr };
}

bool rw::Events::init() {
    if (nullptr != g_events) {
        warn("Event system already initialised.");
        return true;
    }

    g_events = new Events();
    return true;
}

void rw::Events::shutdown() {
    if (nullptr == g_events) {
        warn("Event system already shut down.");
        return;
    }

    delete g_events;
    g_events = nullptr;
}

rw::Events* rw::Events::instance_() {
    return g_events;
}