//
// Created by cjm036653 on 15/08/2025.
//

#ifndef SRC_REDWOLF_MOUSE_HPP
#define SRC_REDWOLF_MOUSE_HPP

#include <GLFW/glfw3.h>
#include <format>
#include <string>

namespace rw::input {
    /**
     * @brief Supported mouse buttons.
     */
    enum class MouseButton {
        left     = GLFW_MOUSE_BUTTON_LEFT,
        right    = GLFW_MOUSE_BUTTON_RIGHT,
        middle   = GLFW_MOUSE_BUTTON_MIDDLE,
        button_4 = GLFW_MOUSE_BUTTON_4,
        button_5 = GLFW_MOUSE_BUTTON_5,
        button_6 = GLFW_MOUSE_BUTTON_6,
        button_7 = GLFW_MOUSE_BUTTON_7,
        button_8 = GLFW_MOUSE_BUTTON_8
    };
} // namespace rw::input

/**
 * @brief std::formatter specialization for rw::input::MouseButton.
 */
template<>
struct std::formatter<rw::input::MouseButton> : std::formatter<std::string> {
    auto format(const rw::input::MouseButton button, std::format_context& ctx) const {
        switch (button) {
        case rw::input::MouseButton::left:
            return std::formatter<std::string>::format("left", ctx);
        case rw::input::MouseButton::right:
            return std::formatter<std::string>::format("right", ctx);
        case rw::input::MouseButton::middle:
            return std::formatter<std::string>::format("middle", ctx);
        case rw::input::MouseButton::button_4:
            return std::formatter<std::string>::format("button_4", ctx);
        case rw::input::MouseButton::button_5:
            return std::formatter<std::string>::format("button_5", ctx);
        case rw::input::MouseButton::button_6:
            return std::formatter<std::string>::format("button_6", ctx);
        case rw::input::MouseButton::button_7:
            return std::formatter<std::string>::format("button_7", ctx);
        case rw::input::MouseButton::button_8:
            return std::formatter<std::string>::format("button_8", ctx);
        default:
            return std::formatter<std::string>::format("unknown button", ctx);
        }
    }
};

#endif // SRC_REDWOLF_MOUSE_HPP
