#pragma once

#include "keyboard.hpp"
#include "mouse.hpp"

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
         * @brief Reset the state of all keyboard keys.
         */
        static void reset_keyboard_keys();

        /**
         * @brief Enable/disable the text mode for keyboard input.
         * @param enabled if true enables text mode, if false disables it.
         */
        static void set_text_mode(bool enabled);

        /**
         * @brief Shutdown the input manager.
         */
        static void shutdown();

        /**
         * @brief Update the state of a keyboard key.
         * @param key Key that changed state.
         * @param pressed true if pressed, false if released.
         */
        static void update_keyboard_key(Key key, bool pressed);

        /**
         * @brief Update the keyboard repeat settings.
         * @param delay Delay after a key press before considering it in repeat mode.
         * @param rate Number of key repeats per second.
         */
        static void update_keyboard_repeat_info(std::chrono::milliseconds delay, i32 rate);

        /**
         * @brief Update the text input by the keyboard.
         * @param text Text input from the keyboard.
         */
        static void update_keyboard_text(std::string_view text);

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

        Mouse    mouse_;    /**< State of the mouse. */
        Keyboard keyboard_; /**< State of the keyboard. */
    };
} // namespace rw