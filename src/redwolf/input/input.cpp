#include "input.hpp"

#include "redwolf/events/events.hpp"
#include "redwolf/input/keyboard.hpp"
#include "redwolf/input/mouse.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/platform/platform.hpp"

#include <algorithm>

namespace {
    rw::Input* g_input{ nullptr }; /**< Global instance. */
}

bool rw::Input::init() {
    if (nullptr != g_input) {
        warn("Input manager already initialised.");
        return true;
    }

    g_input = new Input();
    return true;
}

void rw::Input::reset_keyboard_keys() {
    g_input->keyboard_.keys.fill(false);
}

void rw::Input::set_text_mode(bool enabled) {
    Platform::set_text_mode(enabled);
}

void rw::Input::shutdown() {
    if (nullptr == g_input) {
        warn("Input manager already shut down.");
        return;
    }

    delete g_input;
    g_input = nullptr;
}

void rw::Input::update_keyboard_key(Key key, bool pressed) {
    g_input->keyboard_.keys[static_cast<u8>(key)] = pressed;
    Events::fire(KeyboardKeyEvent{ .key = key, .pressed = pressed });
}

void rw::Input::update_keyboard_repeat_info(std::chrono::milliseconds delay, i32 rate) {
    g_input->keyboard_.repeat_delay = delay;
    g_input->keyboard_.repeat_rate  = rate;
}

void rw::Input::update_keyboard_text(std::string_view text) {
    Events::fire(KeyboardTextEvent{ .text = text });
}

void rw::Input::update_mouse_button(MouseBtn button, bool pressed) {
    g_input->mouse_.buttons[static_cast<usize>(button)] = pressed;
    Events::fire(MouseBtnEvent{ .btn = button, .pressed = pressed });
}

void rw::Input::update_mouse_position(f32 x, f32 y) {
    g_input->mouse_.x = x;
    g_input->mouse_.y = y;
    Events::fire(MouseMoveEvent{ .x = x, .y = y });
}

void rw::Input::update_mouse_scroll(f32 x, f32 y) {
    Events::fire(MouseScrollEvent{ .x = std::clamp(x, -1.0F, 1.0F), .y = std::clamp(y, -1.0F, 1.0F) });
}