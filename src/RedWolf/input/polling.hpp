//
// Created by cjm036653 on 15/08/2025.
//

#ifndef SRC_REDWOLF_INPUT_HPP
#define SRC_REDWOLF_INPUT_HPP

#include "../core/point.hpp"

namespace rw::input {
    /**
     * @brief Check if a keyboard key is currently down.
     * @param key Key to check.
     * @return True if the key is down, false otherwise.
     */
    [[nodiscard]] bool is_key_down(const int key);

    /**
     * @brief Check if a keyboard key is currently up.
     * @param key Key to check.
     * @return True if the key is up, false otherwise.
     */
    [[nodiscard]] bool is_key_up(const int key);

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
