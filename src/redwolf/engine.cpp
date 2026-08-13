#include "engine.hpp"

#include "redwolf/boot/entry_point.hpp"
#include "redwolf/containers/vec.hpp"
#include "redwolf/events/events.hpp"
#include "redwolf/input/input.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/memory/memory.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/platform/platform.hpp"
#include "redwolf/profiler.hpp"

#include <chrono>
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
    Input::shutdown();
    Events::shutdown();
    Memory::shutdown();
    info("Engine shut down.");

    Logger::shutdown();
}

void rw::Engine::loop() {
    usize counter{ 0U };
    auto  last_loop_ts{ std::chrono::high_resolution_clock::now() };
    while (running_) {
        RW_PROFILE_SCOPE

        const auto current_loop_ts{ std::chrono::high_resolution_clock::now() };
        const auto delta_time{ static_cast<f32>(static_cast<f64>((current_loop_ts - last_loop_ts).count()) * nanoseconds_to_seconds) };

        Platform::poll_events();

        // Compute all module updates.
        for (auto& module : modules_) {
            module->on_update(delta_time);
        }

        // Compute all module rendering.
        for (auto& module : modules_) {
            module->on_render(delta_time);
        }

        last_loop_ts = current_loop_ts;

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

    trace("Initialising input manager.");
    if (!Input::init()) {
        error("Failed to initialise input manager.");
        return false;
    }
    info("Input manager initialised.");

    trace("Initialising platform.");
    if (!Platform::init(rw_user::app_name())) {
        error("Failed to initialise platform.");
        return false;
    }
    info("Platform initialised.");

    return true;
}