//
// Created by cjm036653 on 15/08/2025.
//

#include "polling.hpp"

#include "../engine/app.hpp"

#include <GLFW/glfw3.h>

bool rw::input::is_alt_down() {
    return is_key_down(Key::left_alt) || is_key_down(Key::right_alt);
}

bool rw::input::is_control_down() {
    return is_key_down(Key::left_control) || is_key_down(Key::right_control);
}

bool rw::input::is_key_down(const Key key) {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    const auto           state{ static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key))) };
    return KeyState::pressed == state || KeyState::repeated == state;
}

bool rw::input::is_key_up(const Key key) {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    return KeyState::released == static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key)));
}

bool rw::input::is_mouse_button_down(const int button) {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    return KeyState::pressed == static_cast<KeyState>(glfwGetMouseButton(window, button));
}

bool rw::input::is_mouse_button_up(const int button) {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    return KeyState::released == static_cast<KeyState>(glfwGetMouseButton(window, button));
}

bool rw::input::is_shift_down() {
    return is_key_down(Key::left_shift) || is_key_down(Key::right_shift);
}

bool rw::input::is_super_down() {
    return is_key_down(Key::left_super) || is_key_down(Key::right_super);
}

rw::core::Point2D<double> rw::input::mouse_pos() {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    rw::core::Point2D    position;
    glfwGetCursorPos(window, &position.x, &position.y);
    return position;
}

double rw::input::mouse_x() {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    double               x{ 0.0 };
    glfwGetCursorPos(window, &x, nullptr);
    return x;
}

double rw::input::mouse_y() {
    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };
    double               y{ 0.0 };
    glfwGetCursorPos(window, nullptr, &y);
    return y;
}