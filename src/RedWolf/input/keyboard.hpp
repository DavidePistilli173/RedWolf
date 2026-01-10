//
// Created by cjm036653 on 15/08/2025.
//

#ifndef SRC_REDWOLF_KEYS_HPP
#define SRC_REDWOLF_KEYS_HPP

#include <GLFW/glfw3.h>
#include <cstdint>
#include <format>

namespace rw::input {
    /**
     * @brief Key states.
     */
    enum class KeyState : uint8_t {
        released = GLFW_RELEASE, /**< The key or mouse button was released. */
        pressed  = GLFW_PRESS,   /**< The key or mouse button was pressed. */
        repeated = GLFW_REPEAT   /**< The key was held down until it repeated. */
    };

    /**
     * @brief Key codes.
     */
    enum class Key : uint16_t {
        // Printable keys
        space         = GLFW_KEY_SPACE,
        apostrophe    = GLFW_KEY_APOSTROPHE, /* ' */
        comma         = GLFW_KEY_COMMA,      /* , */
        minus         = GLFW_KEY_MINUS,      /* - */
        period        = GLFW_KEY_PERIOD,     /* . */
        slash         = GLFW_KEY_SLASH,      /* / */
        num_0         = GLFW_KEY_0,
        num_1         = GLFW_KEY_1,
        num_2         = GLFW_KEY_2,
        num_3         = GLFW_KEY_3,
        num_4         = GLFW_KEY_4,
        num_5         = GLFW_KEY_5,
        num_6         = GLFW_KEY_6,
        num_7         = GLFW_KEY_7,
        num_8         = GLFW_KEY_8,
        num_9         = GLFW_KEY_9,
        semicolon     = GLFW_KEY_SEMICOLON, /* ; */
        equal         = GLFW_KEY_EQUAL,     /* = */
        a             = GLFW_KEY_A,
        b             = GLFW_KEY_B,
        c             = GLFW_KEY_C,
        d             = GLFW_KEY_D,
        e             = GLFW_KEY_E,
        f             = GLFW_KEY_F,
        g             = GLFW_KEY_G,
        h             = GLFW_KEY_H,
        i             = GLFW_KEY_I,
        j             = GLFW_KEY_J,
        k             = GLFW_KEY_K,
        l             = GLFW_KEY_L,
        m             = GLFW_KEY_M,
        n             = GLFW_KEY_N,
        o             = GLFW_KEY_O,
        p             = GLFW_KEY_P,
        q             = GLFW_KEY_Q,
        r             = GLFW_KEY_R,
        s             = GLFW_KEY_S,
        t             = GLFW_KEY_T,
        u             = GLFW_KEY_U,
        v             = GLFW_KEY_V,
        w             = GLFW_KEY_W,
        x             = GLFW_KEY_X,
        y             = GLFW_KEY_Y,
        z             = GLFW_KEY_Z,
        left_bracket  = GLFW_KEY_LEFT_BRACKET,  /* [ */
        backslash     = GLFW_KEY_BACKSLASH,     /* \ */
        right_bracket = GLFW_KEY_RIGHT_BRACKET, /* ] */
        grave_accent  = GLFW_KEY_GRAVE_ACCENT,  /* ` */
        world_1       = GLFW_KEY_WORLD_1,       /* non-US #1 */
        world_2       = GLFW_KEY_WORLD_2,       /* non-US #2 */

        // Function keys
        escape          = GLFW_KEY_ESCAPE,
        enter           = GLFW_KEY_ENTER,
        tab             = GLFW_KEY_TAB,
        backspace       = GLFW_KEY_BACKSPACE,
        insert          = GLFW_KEY_INSERT,
        del             = GLFW_KEY_DELETE,
        right           = GLFW_KEY_RIGHT,
        left            = GLFW_KEY_LEFT,
        down            = GLFW_KEY_DOWN,
        up              = GLFW_KEY_UP,
        page_up         = GLFW_KEY_PAGE_UP,
        page_down       = GLFW_KEY_PAGE_DOWN,
        home            = GLFW_KEY_HOME,
        end             = GLFW_KEY_END,
        caps_lock       = GLFW_KEY_CAPS_LOCK,
        scroll_lock     = GLFW_KEY_SCROLL_LOCK,
        num_lock        = GLFW_KEY_NUM_LOCK,
        print_screen    = GLFW_KEY_PRINT_SCREEN,
        pause           = GLFW_KEY_PAUSE,
        f1              = GLFW_KEY_F1,
        f2              = GLFW_KEY_F2,
        f3              = GLFW_KEY_F3,
        f4              = GLFW_KEY_F4,
        f5              = GLFW_KEY_F5,
        f6              = GLFW_KEY_F6,
        f7              = GLFW_KEY_F7,
        f8              = GLFW_KEY_F8,
        f9              = GLFW_KEY_F9,
        f10             = GLFW_KEY_F10,
        f11             = GLFW_KEY_F11,
        f12             = GLFW_KEY_F12,
        f13             = GLFW_KEY_F13,
        f14             = GLFW_KEY_F14,
        f15             = GLFW_KEY_F15,
        f16             = GLFW_KEY_F16,
        f17             = GLFW_KEY_F17,
        f18             = GLFW_KEY_F18,
        f19             = GLFW_KEY_F19,
        f20             = GLFW_KEY_F20,
        f21             = GLFW_KEY_F21,
        f22             = GLFW_KEY_F22,
        f23             = GLFW_KEY_F23,
        f24             = GLFW_KEY_F24,
        f25             = GLFW_KEY_F25,
        keypad_0        = GLFW_KEY_KP_0,
        keypad_1        = GLFW_KEY_KP_1,
        keypad_2        = GLFW_KEY_KP_2,
        keypad_3        = GLFW_KEY_KP_3,
        keypad_4        = GLFW_KEY_KP_4,
        keypad_5        = GLFW_KEY_KP_5,
        keypad_6        = GLFW_KEY_KP_6,
        keypad_7        = GLFW_KEY_KP_7,
        keypad_8        = GLFW_KEY_KP_8,
        keypad_9        = GLFW_KEY_KP_9,
        keypad_decimal  = GLFW_KEY_KP_DECIMAL,
        keypad_divide   = GLFW_KEY_KP_DIVIDE,
        keypad_multiply = GLFW_KEY_KP_MULTIPLY,
        keypad_subtract = GLFW_KEY_KP_SUBTRACT,
        keypad_add      = GLFW_KEY_KP_ADD,
        keypad_enter    = GLFW_KEY_KP_ENTER,
        keypad_equal    = GLFW_KEY_KP_EQUAL,
        left_shift      = GLFW_KEY_LEFT_SHIFT,
        left_control    = GLFW_KEY_LEFT_CONTROL,
        left_alt        = GLFW_KEY_LEFT_ALT,
        left_super      = GLFW_KEY_LEFT_SUPER,
        right_shift     = GLFW_KEY_RIGHT_SHIFT,
        right_control   = GLFW_KEY_RIGHT_CONTROL,
        right_alt       = GLFW_KEY_RIGHT_ALT,
        right_super     = GLFW_KEY_RIGHT_SUPER,
        menu            = GLFW_KEY_MENU,
    };

} // namespace rw::input

/**
 * @brief std::formatter specialization for rw::input::Key.
 */
template<>
struct std::formatter<rw::input::Key> : std::formatter<std::string> {
    auto format(const rw::input::Key key, std::format_context& ctx) const {
        switch (key) {
        case rw::input::Key::space:
            return std::formatter<std::string>::format("space", ctx);
        case rw::input::Key::apostrophe:
            return std::formatter<std::string>::format("apostrophe", ctx);
        case rw::input::Key::comma:
            return std::formatter<std::string>::format("comma", ctx);
        case rw::input::Key::minus:
            return std::formatter<std::string>::format("minus", ctx);
        case rw::input::Key::period:
            return std::formatter<std::string>::format("period", ctx);
        case rw::input::Key::slash:
            return std::formatter<std::string>::format("slash", ctx);
        case rw::input::Key::num_0:
            return std::formatter<std::string>::format("num_0", ctx);
        case rw::input::Key::num_1:
            return std::formatter<std::string>::format("num_1", ctx);
        case rw::input::Key::num_2:
            return std::formatter<std::string>::format("num_2", ctx);
        case rw::input::Key::num_3:
            return std::formatter<std::string>::format("num_3", ctx);
        case rw::input::Key::num_4:
            return std::formatter<std::string>::format("num_4", ctx);
        case rw::input::Key::num_5:
            return std::formatter<std::string>::format("num_5", ctx);
        case rw::input::Key::num_6:
            return std::formatter<std::string>::format("num_6", ctx);
        case rw::input::Key::num_7:
            return std::formatter<std::string>::format("num_7", ctx);
        case rw::input::Key::num_8:
            return std::formatter<std::string>::format("num_8", ctx);
        case rw::input::Key::num_9:
            return std::formatter<std::string>::format("num_9", ctx);
        case rw::input::Key::semicolon:
            return std::formatter<std::string>::format("semicolon", ctx);
        case rw::input::Key::equal:
            return std::formatter<std::string>::format("equal", ctx);
        case rw::input::Key::a:
            return std::formatter<std::string>::format("a", ctx);
        case rw::input::Key::b:
            return std::formatter<std::string>::format("b", ctx);
        case rw::input::Key::c:
            return std::formatter<std::string>::format("c", ctx);
        case rw::input::Key::d:
            return std::formatter<std::string>::format("d", ctx);
        case rw::input::Key::e:
            return std::formatter<std::string>::format("e", ctx);
        case rw::input::Key::f:
            return std::formatter<std::string>::format("f", ctx);
        case rw::input::Key::g:
            return std::formatter<std::string>::format("g", ctx);
        case rw::input::Key::h:
            return std::formatter<std::string>::format("h", ctx);
        case rw::input::Key::i:
            return std::formatter<std::string>::format("i", ctx);
        case rw::input::Key::j:
            return std::formatter<std::string>::format("j", ctx);
        case rw::input::Key::k:
            return std::formatter<std::string>::format("k", ctx);
        case rw::input::Key::l:
            return std::formatter<std::string>::format("l", ctx);
        case rw::input::Key::m:
            return std::formatter<std::string>::format("m", ctx);
        case rw::input::Key::n:
            return std::formatter<std::string>::format("n", ctx);
        case rw::input::Key::o:
            return std::formatter<std::string>::format("o", ctx);
        case rw::input::Key::p:
            return std::formatter<std::string>::format("p", ctx);
        case rw::input::Key::q:
            return std::formatter<std::string>::format("q", ctx);
        case rw::input::Key::r:
            return std::formatter<std::string>::format("r", ctx);
        case rw::input::Key::s:
            return std::formatter<std::string>::format("s", ctx);
        case rw::input::Key::t:
            return std::formatter<std::string>::format("t", ctx);
        case rw::input::Key::u:
            return std::formatter<std::string>::format("u", ctx);
        case rw::input::Key::v:
            return std::formatter<std::string>::format("v", ctx);
        case rw::input::Key::w:
            return std::formatter<std::string>::format("w", ctx);
        case rw::input::Key::x:
            return std::formatter<std::string>::format("x", ctx);
        case rw::input::Key::y:
            return std::formatter<std::string>::format("y", ctx);
        case rw::input::Key::z:
            return std::formatter<std::string>::format("z", ctx);
        case rw::input::Key::left_bracket:
            return std::formatter<std::string>::format("left_bracket", ctx);
        case rw::input::Key::backslash:
            return std::formatter<std::string>::format("backslash", ctx);
        case rw::input::Key::right_bracket:
            return std::formatter<std::string>::format("right_bracket", ctx);
        case rw::input::Key::grave_accent:
            return std::formatter<std::string>::format("grave_accent", ctx);
        case rw::input::Key::world_1:
            return std::formatter<std::string>::format("world_1", ctx);
        case rw::input::Key::world_2:
            return std::formatter<std::string>::format("world_2", ctx);
        case rw::input::Key::escape:
            return std::formatter<std::string>::format("escape", ctx);
        case rw::input::Key::enter:
            return std::formatter<std::string>::format("enter", ctx);
        case rw::input::Key::tab:
            return std::formatter<std::string>::format("tab", ctx);
        case rw::input::Key::backspace:
            return std::formatter<std::string>::format("backspace", ctx);
        case rw::input::Key::insert:
            return std::formatter<std::string>::format("insert", ctx);
        case rw::input::Key::del:
            return std::formatter<std::string>::format("del", ctx);
        case rw::input::Key::right:
            return std::formatter<std::string>::format("right", ctx);
        case rw::input::Key::left:
            return std::formatter<std::string>::format("left", ctx);
        case rw::input::Key::down:
            return std::formatter<std::string>::format("down", ctx);
        case rw::input::Key::up:
            return std::formatter<std::string>::format("up", ctx);
        case rw::input::Key::page_up:
            return std::formatter<std::string>::format("page_up", ctx);
        case rw::input::Key::page_down:
            return std::formatter<std::string>::format("page_down", ctx);
        case rw::input::Key::home:
            return std::formatter<std::string>::format("home", ctx);
        case rw::input::Key::end:
            return std::formatter<std::string>::format("end", ctx);
        case rw::input::Key::caps_lock:
            return std::formatter<std::string>::format("caps_lock", ctx);
        case rw::input::Key::scroll_lock:
            return std::formatter<std::string>::format("scroll_lock", ctx);
        case rw::input::Key::num_lock:
            return std::formatter<std::string>::format("num_lock", ctx);
        case rw::input::Key::print_screen:
            return std::formatter<std::string>::format("print_screen", ctx);
        case rw::input::Key::pause:
            return std::formatter<std::string>::format("pause", ctx);
        case rw::input::Key::f1:
            return std::formatter<std::string>::format("f1", ctx);
        case rw::input::Key::f2:
            return std::formatter<std::string>::format("f2", ctx);
        case rw::input::Key::f3:
            return std::formatter<std::string>::format("f3", ctx);
        case rw::input::Key::f4:
            return std::formatter<std::string>::format("f4", ctx);
        case rw::input::Key::f5:
            return std::formatter<std::string>::format("f5", ctx);
        case rw::input::Key::f6:
            return std::formatter<std::string>::format("f6", ctx);
        case rw::input::Key::f7:
            return std::formatter<std::string>::format("f7", ctx);
        case rw::input::Key::f8:
            return std::formatter<std::string>::format("f8", ctx);
        case rw::input::Key::f9:
            return std::formatter<std::string>::format("f9", ctx);
        case rw::input::Key::f10:
            return std::formatter<std::string>::format("f10", ctx);
        case rw::input::Key::f11:
            return std::formatter<std::string>::format("f11", ctx);
        case rw::input::Key::f12:
            return std::formatter<std::string>::format("f12", ctx);
        case rw::input::Key::f13:
            return std::formatter<std::string>::format("f13", ctx);
        case rw::input::Key::f14:
            return std::formatter<std::string>::format("f14", ctx);
        case rw::input::Key::f15:
            return std::formatter<std::string>::format("f15", ctx);
        case rw::input::Key::f16:
            return std::formatter<std::string>::format("f16", ctx);
        case rw::input::Key::f17:
            return std::formatter<std::string>::format("f17", ctx);
        case rw::input::Key::f18:
            return std::formatter<std::string>::format("f18", ctx);
        case rw::input::Key::f19:
            return std::formatter<std::string>::format("f19", ctx);
        case rw::input::Key::f20:
            return std::formatter<std::string>::format("f20", ctx);
        case rw::input::Key::f21:
            return std::formatter<std::string>::format("f21", ctx);
        case rw::input::Key::f22:
            return std::formatter<std::string>::format("f22", ctx);
        case rw::input::Key::f23:
            return std::formatter<std::string>::format("f23", ctx);
        case rw::input::Key::f24:
            return std::formatter<std::string>::format("f24", ctx);
        case rw::input::Key::f25:
            return std::formatter<std::string>::format("f25", ctx);
        case rw::input::Key::keypad_0:
            return std::formatter<std::string>::format("keypad_0", ctx);
        case rw::input::Key::keypad_1:
            return std::formatter<std::string>::format("keypad_1", ctx);
        case rw::input::Key::keypad_2:
            return std::formatter<std::string>::format("keypad_2", ctx);
        case rw::input::Key::keypad_3:
            return std::formatter<std::string>::format("keypad_3", ctx);
        case rw::input::Key::keypad_4:
            return std::formatter<std::string>::format("keypad_4", ctx);
        case rw::input::Key::keypad_5:
            return std::formatter<std::string>::format("keypad_5", ctx);
        case rw::input::Key::keypad_6:
            return std::formatter<std::string>::format("keypad_6", ctx);
        case rw::input::Key::keypad_7:
            return std::formatter<std::string>::format("keypad_7", ctx);
        case rw::input::Key::keypad_8:
            return std::formatter<std::string>::format("keypad_8", ctx);
        case rw::input::Key::keypad_9:
            return std::formatter<std::string>::format("keypad_9", ctx);
        case rw::input::Key::keypad_decimal:
            return std::formatter<std::string>::format("decimal", ctx);
        case rw::input::Key::keypad_divide:
            return std::formatter<std::string>::format("divide", ctx);
        case rw::input::Key::keypad_multiply:
            return std::formatter<std::string>::format("multiply", ctx);
        case rw::input::Key::keypad_subtract:
            return std::formatter<std::string>::format("subtract", ctx);
        case rw::input::Key::keypad_add:
            return std::formatter<std::string>::format("add", ctx);
        case rw::input::Key::keypad_enter:
            return std::formatter<std::string>::format("keypad_enter", ctx);
        case rw::input::Key::keypad_equal:
            return std::formatter<std::string>::format("keypad_equal", ctx);
        case rw::input::Key::left_shift:
            return std::formatter<std::string>::format("left_shift", ctx);
        case rw::input::Key::left_control:
            return std::formatter<std::string>::format("left_control", ctx);
        case rw::input::Key::left_alt:
            return std::formatter<std::string>::format("left_alt", ctx);
        case rw::input::Key::left_super:
            return std::formatter<std::string>::format("left_super", ctx);
        case rw::input::Key::right_shift:
            return std::formatter<std::string>::format("right_shift", ctx);
        case rw::input::Key::right_control:
            return std::formatter<std::string>::format("right_control", ctx);
        case rw::input::Key::right_alt:
            return std::formatter<std::string>::format("right_alt", ctx);
        case rw::input::Key::right_super:
            return std::formatter<std::string>::format("right_super", ctx);
        case rw::input::Key::menu:
            return std::formatter<std::string>::format("menu", ctx);
        default:
            return std::formatter<std::string>::format("unknown key", ctx);
        }
    }
};

#endif // SRC_REDWOLF_KEYS_HPP
