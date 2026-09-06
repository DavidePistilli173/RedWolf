#pragma once

#include "common.hpp"
#include "containers/vec.hpp"
#include "events/events.hpp"
#include "memory/generic_allocator.hpp"
#include "memory/ptr.hpp"
#include "module.hpp"
#include "platform/platform_common.hpp"

#include <string_view>

namespace rw {
    /**
     * @brief Main class for the engine.
     */
    class Engine {
     public:
        static constexpr f64 target_framerate{ 60.0 }; /**< Target frames per second. */

        /**
         * @brief Target loop time.
         */
        static constexpr std::chrono::nanoseconds target_frame_time{ static_cast<u64>(1 / (nanoseconds_to_seconds * target_framerate)) };

        Engine();
        ~Engine();

        Engine(const Engine&)            = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&&)            = delete;
        Engine& operator=(Engine&&) = delete;

        /**
         * @brief Initialise the engine.
         * @return true if successfull, false otherwise.
         */
        [[nodiscard]] bool init();

        /**
         * @brief Engine main loop.
         */
        void loop();

     private:
        /**
         * @brief Initialise the user modules.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_modules_();

        /**
         * @brief Initialise the engine sub-systems.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_subsystems_();

        bool             running_{ true };                   /**< Main loop conditino flag. */
        Vec<Ptr<Module>> modules_{ MemoryCategory::engine }; /**< User modules. */

        Connection<WindowCloseEvent> window_close_event_; /**< Connection for the window close event. */
    };
} // namespace rw