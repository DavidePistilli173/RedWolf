#pragma once

namespace rw {
    /**
     * @brief Event system.
     */
    class Events {
     public:
        /**
         * @brief Initialise the event system.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Shutdown the event system.
         */
        static void shutdown();

     private:
        Events() = default;
    };
} // namespace rw