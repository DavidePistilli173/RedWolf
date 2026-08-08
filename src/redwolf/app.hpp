#pragma once

#include "redwolf/common.hpp"

namespace rw {
    /**
     * @brief Base application class, meant to be inherited by user application classes.
     */
    class App {
     public:
        App()          = default;
        virtual ~App() = default;

        App(const App&)            = delete;
        App& operator=(const App&) = delete;

        App(App&&)            = delete;
        App& operator=(App&&) = delete;

        /**
         * @brief Called at startup to initialise the application.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool init() = 0;

        /**
         * @brief Called on each update tick.
         * @param delta_time Time since the last update. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool on_update(f32 delta_time) = 0;

        /**
         * @brief Called on each render tick.
         * @param delat_time Time since the last render. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool on_render(f32 delta_time) = 0;

     private:
    };
} // namespace rw