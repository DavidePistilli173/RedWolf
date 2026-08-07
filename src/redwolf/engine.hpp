#pragma once

#include <string_view>

namespace rw {
    /**
     * @brief Main class for the engine.
     */
    class Engine {
     public:
        Engine();
        ~Engine();

        Engine(const Engine&)            = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&&)            = delete;
        Engine& operator=(Engine&&) = delete;

        /**
         * @brief Initialise the engine.
         * @param app_name Name of the application.
         * @return true if successfull, false otherwise.
         */
        [[nodiscard]] bool init(std::string_view app_name);

        /**
         * @brief Engine main loop.
         */
        void loop();

     private:
    };
} // namespace rw