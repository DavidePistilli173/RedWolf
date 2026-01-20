module;

#include <GLFW/glfw3.h>
#include <cstdint>
#include <format>

export module redwolf.input.keyboard;

export namespace rw::input {
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
struct std::formatter<rw::input::Key> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::input::Key key, std::format_context& ctx) const {
        switch (key) {
        case rw::input::Key::space:
            return std::format_to(ctx.out(), "space");
        case rw::input::Key::apostrophe:
            return std::format_to(ctx.out(), "apostrophe");
        case rw::input::Key::comma:
            return std::format_to(ctx.out(), "comma");
        case rw::input::Key::minus:
            return std::format_to(ctx.out(), "minus");
        case rw::input::Key::period:
            return std::format_to(ctx.out(), "period");
        case rw::input::Key::slash:
            return std::format_to(ctx.out(), "slash");
        case rw::input::Key::num_0:
            return std::format_to(ctx.out(), "num_0");
        case rw::input::Key::num_1:
            return std::format_to(ctx.out(), "num_1");
        case rw::input::Key::num_2:
            return std::format_to(ctx.out(), "num_2");
        case rw::input::Key::num_3:
            return std::format_to(ctx.out(), "num_3");
        case rw::input::Key::num_4:
            return std::format_to(ctx.out(), "num_4");
        case rw::input::Key::num_5:
            return std::format_to(ctx.out(), "num_5");
        case rw::input::Key::num_6:
            return std::format_to(ctx.out(), "num_6");
        case rw::input::Key::num_7:
            return std::format_to(ctx.out(), "num_7");
        case rw::input::Key::num_8:
            return std::format_to(ctx.out(), "num_8");
        case rw::input::Key::num_9:
            return std::format_to(ctx.out(), "num_9");
        case rw::input::Key::semicolon:
            return std::format_to(ctx.out(), "semicolon");
        case rw::input::Key::equal:
            return std::format_to(ctx.out(), "equal");
        case rw::input::Key::a:
            return std::format_to(ctx.out(), "a");
        case rw::input::Key::b:
            return std::format_to(ctx.out(), "b");
        case rw::input::Key::c:
            return std::format_to(ctx.out(), "c");
        case rw::input::Key::d:
            return std::format_to(ctx.out(), "d");
        case rw::input::Key::e:
            return std::format_to(ctx.out(), "e");
        case rw::input::Key::f:
            return std::format_to(ctx.out(), "f");
        case rw::input::Key::g:
            return std::format_to(ctx.out(), "g");
        case rw::input::Key::h:
            return std::format_to(ctx.out(), "h");
        case rw::input::Key::i:
            return std::format_to(ctx.out(), "i");
        case rw::input::Key::j:
            return std::format_to(ctx.out(), "j");
        case rw::input::Key::k:
            return std::format_to(ctx.out(), "k");
        case rw::input::Key::l:
            return std::format_to(ctx.out(), "l");
        case rw::input::Key::m:
            return std::format_to(ctx.out(), "m");
        case rw::input::Key::n:
            return std::format_to(ctx.out(), "n");
        case rw::input::Key::o:
            return std::format_to(ctx.out(), "o");
        case rw::input::Key::p:
            return std::format_to(ctx.out(), "p");
        case rw::input::Key::q:
            return std::format_to(ctx.out(), "q");
        case rw::input::Key::r:
            return std::format_to(ctx.out(), "r");
        case rw::input::Key::s:
            return std::format_to(ctx.out(), "s");
        case rw::input::Key::t:
            return std::format_to(ctx.out(), "t");
        case rw::input::Key::u:
            return std::format_to(ctx.out(), "u");
        case rw::input::Key::v:
            return std::format_to(ctx.out(), "v");
        case rw::input::Key::w:
            return std::format_to(ctx.out(), "w");
        case rw::input::Key::x:
            return std::format_to(ctx.out(), "x");
        case rw::input::Key::y:
            return std::format_to(ctx.out(), "y");
        case rw::input::Key::z:
            return std::format_to(ctx.out(), "z");
        case rw::input::Key::left_bracket:
            return std::format_to(ctx.out(), "left_bracket");
        case rw::input::Key::backslash:
            return std::format_to(ctx.out(), "backslash");
        case rw::input::Key::right_bracket:
            return std::format_to(ctx.out(), "right_bracket");
        case rw::input::Key::grave_accent:
            return std::format_to(ctx.out(), "grave_accent");
        case rw::input::Key::world_1:
            return std::format_to(ctx.out(), "world_1");
        case rw::input::Key::world_2:
            return std::format_to(ctx.out(), "world_2");
        case rw::input::Key::escape:
            return std::format_to(ctx.out(), "escape");
        case rw::input::Key::enter:
            return std::format_to(ctx.out(), "enter");
        case rw::input::Key::tab:
            return std::format_to(ctx.out(), "tab");
        case rw::input::Key::backspace:
            return std::format_to(ctx.out(), "backspace");
        case rw::input::Key::insert:
            return std::format_to(ctx.out(), "insert");
        case rw::input::Key::del:
            return std::format_to(ctx.out(), "del");
        case rw::input::Key::right:
            return std::format_to(ctx.out(), "right");
        case rw::input::Key::left:
            return std::format_to(ctx.out(), "left");
        case rw::input::Key::down:
            return std::format_to(ctx.out(), "down");
        case rw::input::Key::up:
            return std::format_to(ctx.out(), "up");
        case rw::input::Key::page_up:
            return std::format_to(ctx.out(), "page_up");
        case rw::input::Key::page_down:
            return std::format_to(ctx.out(), "page_down");
        case rw::input::Key::home:
            return std::format_to(ctx.out(), "home");
        case rw::input::Key::end:
            return std::format_to(ctx.out(), "end");
        case rw::input::Key::caps_lock:
            return std::format_to(ctx.out(), "caps_lock");
        case rw::input::Key::scroll_lock:
            return std::format_to(ctx.out(), "scroll_lock");
        case rw::input::Key::num_lock:
            return std::format_to(ctx.out(), "num_lock");
        case rw::input::Key::print_screen:
            return std::format_to(ctx.out(), "print_screen");
        case rw::input::Key::pause:
            return std::format_to(ctx.out(), "pause");
        case rw::input::Key::f1:
            return std::format_to(ctx.out(), "f1");
        case rw::input::Key::f2:
            return std::format_to(ctx.out(), "f2");
        case rw::input::Key::f3:
            return std::format_to(ctx.out(), "f3");
        case rw::input::Key::f4:
            return std::format_to(ctx.out(), "f4");
        case rw::input::Key::f5:
            return std::format_to(ctx.out(), "f5");
        case rw::input::Key::f6:
            return std::format_to(ctx.out(), "f6");
        case rw::input::Key::f7:
            return std::format_to(ctx.out(), "f7");
        case rw::input::Key::f8:
            return std::format_to(ctx.out(), "f8");
        case rw::input::Key::f9:
            return std::format_to(ctx.out(), "f9");
        case rw::input::Key::f10:
            return std::format_to(ctx.out(), "f10");
        case rw::input::Key::f11:
            return std::format_to(ctx.out(), "f11");
        case rw::input::Key::f12:
            return std::format_to(ctx.out(), "f12");
        case rw::input::Key::f13:
            return std::format_to(ctx.out(), "f13");
        case rw::input::Key::f14:
            return std::format_to(ctx.out(), "f14");
        case rw::input::Key::f15:
            return std::format_to(ctx.out(), "f15");
        case rw::input::Key::f16:
            return std::format_to(ctx.out(), "f16");
        case rw::input::Key::f17:
            return std::format_to(ctx.out(), "f17");
        case rw::input::Key::f18:
            return std::format_to(ctx.out(), "f18");
        case rw::input::Key::f19:
            return std::format_to(ctx.out(), "f19");
        case rw::input::Key::f20:
            return std::format_to(ctx.out(), "f20");
        case rw::input::Key::f21:
            return std::format_to(ctx.out(), "f21");
        case rw::input::Key::f22:
            return std::format_to(ctx.out(), "f22");
        case rw::input::Key::f23:
            return std::format_to(ctx.out(), "f23");
        case rw::input::Key::f24:
            return std::format_to(ctx.out(), "f24");
        case rw::input::Key::f25:
            return std::format_to(ctx.out(), "f25");
        case rw::input::Key::keypad_0:
            return std::format_to(ctx.out(), "keypad_0");
        case rw::input::Key::keypad_1:
            return std::format_to(ctx.out(), "keypad_1");
        case rw::input::Key::keypad_2:
            return std::format_to(ctx.out(), "keypad_2");
        case rw::input::Key::keypad_3:
            return std::format_to(ctx.out(), "keypad_3");
        case rw::input::Key::keypad_4:
            return std::format_to(ctx.out(), "keypad_4");
        case rw::input::Key::keypad_5:
            return std::format_to(ctx.out(), "keypad_5");
        case rw::input::Key::keypad_6:
            return std::format_to(ctx.out(), "keypad_6");
        case rw::input::Key::keypad_7:
            return std::format_to(ctx.out(), "keypad_7");
        case rw::input::Key::keypad_8:
            return std::format_to(ctx.out(), "keypad_8");
        case rw::input::Key::keypad_9:
            return std::format_to(ctx.out(), "keypad_9");
        case rw::input::Key::keypad_decimal:
            return std::format_to(ctx.out(), "decimal");
        case rw::input::Key::keypad_divide:
            return std::format_to(ctx.out(), "divide");
        case rw::input::Key::keypad_multiply:
            return std::format_to(ctx.out(), "multiply");
        case rw::input::Key::keypad_subtract:
            return std::format_to(ctx.out(), "subtract");
        case rw::input::Key::keypad_add:
            return std::format_to(ctx.out(), "add");
        case rw::input::Key::keypad_enter:
            return std::format_to(ctx.out(), "keypad_enter");
        case rw::input::Key::keypad_equal:
            return std::format_to(ctx.out(), "keypad_equal");
        case rw::input::Key::left_shift:
            return std::format_to(ctx.out(), "left_shift");
        case rw::input::Key::left_control:
            return std::format_to(ctx.out(), "left_control");
        case rw::input::Key::left_alt:
            return std::format_to(ctx.out(), "left_alt");
        case rw::input::Key::left_super:
            return std::format_to(ctx.out(), "left_super");
        case rw::input::Key::right_shift:
            return std::format_to(ctx.out(), "right_shift");
        case rw::input::Key::right_control:
            return std::format_to(ctx.out(), "right_control");
        case rw::input::Key::right_alt:
            return std::format_to(ctx.out(), "right_alt");
        case rw::input::Key::right_super:
            return std::format_to(ctx.out(), "right_super");
        case rw::input::Key::menu:
            return std::format_to(ctx.out(), "menu");
        default:
            return std::format_to(ctx.out(), "unknown key");
        }
    }
};
