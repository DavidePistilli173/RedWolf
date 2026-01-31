module;

#include <GLFW/glfw3.h>
#include <format>
#include <string>

export module redwolf.input.mouse;

export namespace rw::input {
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
struct std::formatter<rw::input::MouseButton> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::input::MouseButton button, std::format_context& ctx) const {
        switch (button) {
        case rw::input::MouseButton::left:
            return std::format_to(ctx.out(), "left");
        case rw::input::MouseButton::right:
            return std::format_to(ctx.out(), "right");
        case rw::input::MouseButton::middle:
            return std::format_to(ctx.out(), "middle");
        case rw::input::MouseButton::button_4:
            return std::format_to(ctx.out(), "button_4");
        case rw::input::MouseButton::button_5:
            return std::format_to(ctx.out(), "button_5");
        case rw::input::MouseButton::button_6:
            return std::format_to(ctx.out(), "button_6");
        case rw::input::MouseButton::button_7:
            return std::format_to(ctx.out(), "button_7");
        case rw::input::MouseButton::button_8:
            return std::format_to(ctx.out(), "button_8");
        default:
            return std::format_to(ctx.out(), "unknown button");
        }
    }
};
