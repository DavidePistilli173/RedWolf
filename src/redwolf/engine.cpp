#include "engine.hpp"

#include "boot/entry_point.hpp"
#include "containers/vec.hpp"
#include "input/input.hpp"
#include "logger.hpp"
#include "math/random.hpp"
#include "memory/memory.hpp"
#include "memory/memory_pool.hpp"
#include "platform/platform.hpp"
#include "profiler.hpp"
#include "renderer/backend/vulkan_renderer.hpp"
#include "renderer/render_packet.hpp"
#include "renderer/renderer.hpp"
#include "user_data.hpp"
#include "version_info.hpp"

#include <chrono>
#include <limits>
#include <thread>
#include <vector>

rw::Engine::Engine() {
    Logger::init();
    info("Starting RedWolf v{}.{}.{}.{}", rw::version.major, rw::version.minor, rw::version.patch, rw::version.build);
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

    window_close_event_ = Events::subscribe<WindowCloseEvent>([this]([[maybe_unused]] const WindowCloseEvent& event) {
        running_ = false;
        return false;
    });

    return true;
}

rw::Engine::~Engine() {
    RW_PROFILE_SCOPE

    info("Shutting down engine.");

    window_close_event_.unsubscribe();

    // Shut down all user modules.
    for (auto& module : modules_) {
        module->on_close();
    }
    modules_.reset();

    Renderer::shutdown();
    Platform::shutdown();
    Input::shutdown();
    Events::shutdown();
    Memory::shutdown();
    UserData::shutdown();
    Random::shutdown();
    info("Engine shut down.");

    Logger::shutdown();
}

void rw::Engine::loop() {
    auto last_loop_ts{ std::chrono::high_resolution_clock::now() };

    while (running_) {
        RW_PROFILE_SCOPE

        const auto current_loop_ts{ std::chrono::high_resolution_clock::now() };
        const auto elapsed_ns{ current_loop_ts - last_loop_ts };
        const auto delta_time{ static_cast<f32>(static_cast<f64>((elapsed_ns).count()) * nanoseconds_to_seconds) };

        Platform::poll_events();

        // Compute all module updates.
        for (auto& module : modules_) {
            module->on_update(delta_time);
        }

        // Compute all module rendering.
        for (auto& module : modules_) {
            module->on_render(delta_time);
        }

        // Actually render the frame.
        RenderPacket render_packet{ .delta_time = delta_time };
        if (!Renderer::draw_frame(render_packet)) {
            fatal("Renderer failed to draw frame.");
            running_ = false;
        }

        last_loop_ts = current_loop_ts;

        const auto end_ts{ std::chrono::high_resolution_clock::now() };
        const auto elapsed{ end_ts - current_loop_ts };
        if (elapsed > target_frame_time) {
            warn("Main loop overrun by: '{}'", elapsed - target_frame_time);
        } else if (elapsed < target_frame_time) {
            RW_PROFILE_SCOPE
            std::this_thread::sleep_for(target_frame_time - elapsed);
        }

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
    trace("Initialising random number generator.");
    Random::init(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    info("Random number generator initialised.");

    trace("Initialising user data manager.");
    if (!UserData::init()) {
        error("Failed to initialise user data manager.");
        return false;
    }
    info("User data manager initialised.");

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
    if (!Platform::init()) {
        error("Failed to initialise platform.");
        return false;
    }
    info("Platform initialised.");

    trace("Initialising renderer.");
    if (!Renderer::init()) {
        error("Failed to initialise renderer.");
        return false;
    }
    info("Renderer initialised.");

    return true;
}