#pragma once

#include "redwolf/input/mouse.hpp"

namespace rw {
    /**
     * @brief Input manager.
     */
    class Input {
     public:
        /**
         * @brief Initialise the input manager.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Shutdown the input manager.
         */
        static void shutdown();

        /**
         * @brief Update the state of a mouse button.
         * @param button Button to update.
         * @param pressed true if pressed, false if released.
         */
        static void update_mouse_button(MouseBtn button, bool pressed);

        /**
         * @brief Update the position of the mouse.
         * @param x New X coordinate (from the top-left corner).
         * @param y New Y coordinate (from the top-left corner).
         */
        static void update_mouse_position(f32 x, f32 y);

        /**
         * @brief Update mouse scrolling.
         * @param x Horizontal scrolling. Must be normalized in [-1,1].
         * @param y Vertical scrolling. Must be normalized in [-1,1].
         */
        static void update_mouse_scroll(f32 x, f32 y);

     private:
        Input() = default;

        Mouse mouse_; /**< State of the mouse. */
    };
} // namespace rw