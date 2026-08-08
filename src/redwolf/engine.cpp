#include "engine.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/profiler.hpp"

#include <vector>

rw::Engine::Engine() {
    Logger::init();
    info("Logger initialised.");
}

bool rw::Engine::init(std::string_view app_name) {
    trace("Initialising platform.");
    if (!Platform::init(app_name)) {
        error("Failed to initialise platform.");
        return false;
    }
    info("Platform initialised.");

    return true;
}

rw::Engine::~Engine() {
    Platform::shutdown();
    Logger::shutdown();
}

void rw::Engine::loop() {
    usize counter{ 0U };
    while (counter < 100'000) {
        Platform::poll_events();
        ++counter;
    }
}