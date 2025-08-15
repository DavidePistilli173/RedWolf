//
// Created by cjm036653 on 15/08/2025.
//

#ifndef SRC_REDWOLF_INPUT_HPP
#define SRC_REDWOLF_INPUT_HPP

#include "../core/point.hpp"
#include "keyboard.hpp"

namespace rw::input {
    /**
     * @brief Check whether the left or right alt key is currently down.
     * @return True if any of the alt keys is down, false otherwise.
     */
    [[nodiscard]] bool is_alt_down();

    /**
     * @brief Check whether the left or right control key is currently down.
     * @return True if any of the control keys is down, false otherwise.
     */
    [[nodiscard]] bool is_control_down();

    /**
     * @brief Check if a keyboard key is currently down.
     * @param key Key to check.
     * @return True if the key is down, false otherwise.
     */
    [[nodiscard]] bool is_key_down(const Key key);

    /**
     * @brief Check if a keyboard key is currently up.
     * @param key Key to check.
     * @return True if the key is up, false otherwise.
     */
    [[nodiscard]] bool is_key_up(const Key key);

    /**
     * @brief Check if a mouse button is currently down.
     * @param button Button to check.
     * @return True if the button is down, false otherwise.
     */
    [[nodiscard]] bool is_mouse_button_down(const int button);

    /**
     * @brief Check if a mouse button is currently up.
     * @param button Button to check.
     * @return True if the button is up, false otherwise.
     */
    [[nodiscard]] bool is_mouse_button_up(const int button);

    /**
     * @brief Check whether the left or right shift key is currently down.
     * @return True if any of the shift keys is down, false otherwise.
     */
    [[nodiscard]] bool is_shift_down();

    /**
     * @brief Check whether the super key (Windows key or Command key on macOS) is currently down.
     * @return True if the super key is down, false otherwise.
     */
    [[nodiscard]] bool is_super_down();

    /**
     * @brief Get the current mouse position.
     * @return Current mouse position.
     */
    [[nodiscard]] rw::core::Point2D<double> mouse_pos();

    /**
     * @brief Get the current mouse position on the X axis.
     * @return Current mouse X position.
     */
    [[nodiscard]] double mouse_x();

    /**
     * @brief Get the current mouse position on the Y axis.
     * @return Current mouse Y position.
     */
    [[nodiscard]] double mouse_y();
} // namespace rw::input

#endif // SRC_REDWOLF_INPUT_HPP
