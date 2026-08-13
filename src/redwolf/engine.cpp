#include "engine.hpp"

#include "redwolf/boot/entry_point.hpp"
#include "redwolf/containers/vec.hpp"
#include "redwolf/events/events.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/memory/memory.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/profiler.hpp"

#include <limits>
#include <vector>

rw::Engine::Engine() {
    Logger::init();
    info("Logger initialised.");
}

bool rw::Engine::init() {
    RW_PROFILE_SCOPE

    if (!init_subsystems_()) {
        rw::error("Failed to initialise the engine sub-systems.");
        return false;
    }

    if (!init_modules_()) {
        rw::error("Failed to initialise the user modules.");
        return false;
    }

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
    while (running_) {
        RW_PROFILE_SCOPE

        Platform::poll_events();

        RW_PROFILE_MARK_FRAME
    }
}

bool rw::Engine::init_modules_() {
    modules_ = rw_user::create_modules();

    for (auto& module : modules_) {
        if (!module->init()) {
            rw::error("Failed to initialise module '{}'", module->name());
            return false;
        }
        rw::info("Initialised module '{}'.", module->name());
    }

    return true;
}

bool rw::Engine::init_subsystems_() {
    trace("Initialising platform.");
    if (!Platform::init(rw_user::app_name())) {
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

    trace("Initialising event system.");
    if (!Events::init()) {
        error("Failed to initialise the event system.");
        return false;
    }
    info("Event system initialised.");

    return true;
}