module;

#include <glfw/glfw3.h>

export module redwolf.input.polling;

import redwolf.engine.app;
import redwolf.input.keyboard;
import redwolf.math;
import redwolf.ui.common;

export namespace rw::input {
    /**
     * @brief Check if a keyboard key is currently down.
     * @param key Key to check.
     * @return True if the key is down, false otherwise.
     */
    [[nodiscard]] bool is_key_down(const Key key) {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        const auto           state{ static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key))) };
        return KeyState::pressed == state || KeyState::repeated == state;
    }

    /**
     * @brief Check if a keyboard key is currently up.
     * @param key Key to check.
     * @return True if the key is up, false otherwise.
     */
    [[nodiscard]] bool is_key_up(const Key key) {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        return KeyState::released == static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key)));
    }

    /**
     * @brief Check whether the left or right alt key is currently down.
     * @return True if any of the alt keys is down, false otherwise.
     */
    [[nodiscard]] bool is_alt_down() {
        return is_key_down(Key::left_alt) || is_key_down(Key::right_alt);
    }

    /**
     * @brief Check whether the left or right control key is currently down.
     * @return True if any of the control keys is down, false otherwise.
     */
    [[nodiscard]] bool is_control_down() {
        return is_key_down(Key::left_control) || is_key_down(Key::right_control);
    }

    /**
     * @brief Check if a mouse button is currently down.
     * @param button Button to check.
     * @return True if the button is down, false otherwise.
     */
    [[nodiscard]] bool is_mouse_button_down(const int button) {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        return KeyState::pressed == static_cast<KeyState>(glfwGetMouseButton(window, button));
    }

    /**
     * @brief Check if a mouse button is currently up.
     * @param button Button to check.
     * @return True if the button is up, false otherwise.
     */
    [[nodiscard]] bool is_mouse_button_up(const int button) {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        return KeyState::released == static_cast<KeyState>(glfwGetMouseButton(window, button));
    }

    /**
     * @brief Check whether the left or right shift key is currently down.
     * @return True if any of the shift keys is down, false otherwise.
     */
    [[nodiscard]] bool is_shift_down() {
        return is_key_down(Key::left_shift) || is_key_down(Key::right_shift);
    }

    /**
     * @brief Check whether the super key (Windows key or Command key on macOS) is currently down.
     * @return True if the super key is down, false otherwise.
     */
    [[nodiscard]] bool is_super_down() {
        return is_key_down(Key::left_super) || is_key_down(Key::right_super);
    }

    /**
     * @brief Get the current mouse position.
     * @return Current mouse position.
     */
    [[nodiscard]] rw::math::Vec2 mouse_pos() {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        double               x{ 0.0 };
        double               y{ 0.0 };
        rw::math::Vec2       position;
        glfwGetCursorPos(window, &x, &y);
        position.x = static_cast<float>(x);
        position.y = static_cast<float>(y);
        return position;
    }

    /**
     * @brief Get the current mouse position on the X axis.
     * @return Current mouse X position.
     */
    [[nodiscard]] double mouse_x() {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        double               x{ 0.0 };
        glfwGetCursorPos(window, &x, nullptr);
        return x;
    }

    /**
     * @brief Get the current mouse position on the Y axis.
     * @return Current mouse Y position.
     */
    [[nodiscard]] double mouse_y() {
        rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
        double               y{ 0.0 };
        glfwGetCursorPos(window, nullptr, &y);
        return y;
    }
} // namespace rw::input
