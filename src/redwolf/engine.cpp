#include "engine.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/memory.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/profiler.hpp"

#include <vector>

rw::Engine::Engine() {
    Logger::init();
    info("Logger initialised.");
}

bool rw::Engine::init(std::string_view app_name) {
    RW_PROFILE_SCOPE

    trace("Initialising platform.");
    if (!Platform::init(app_name)) {
        error("Failed to initialise platform.");
        return false;
    }
    info("Platform initialised.");

    trace("Initialising memory manager.");
    if (!Memory::init()) {
        error("Failed to initialise memory manager.");
        return false;
    }
    info("Memory manager initialised.");

    return true;
}

rw::Engine::~Engine() {
    RW_PROFILE_SCOPE

    info("Shutting down engine.");
    Platform::shutdown();
    info("Engine shut down.");

    Logger::shutdown();
}

void rw::Engine::loop() {
    usize counter{ 0U };
    while (counter < 100'000) {
        RW_PROFILE_SCOPE

        Platform::poll_events();
        ++counter;

        RW_PROFILE_MARK_FRAME
    }
}