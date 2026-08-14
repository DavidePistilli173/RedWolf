#pragma once

#include "redwolf/common.hpp"

#include <array>
#include <chrono>
#include <limits>
#include <string_view>

namespace rw {
    /**
     * @brief Keyboard keys.
     */
    enum class Key : u8 {
        invalid,
        esc,
        num_1,
        num_2,
        num_3,
        num_4,
        num_5,
        num_6,
        num_7,
        num_8,
        num_9,
        num_0,
        minus,
        equal,
        backspace,
        tab,
        q,
        w,
        e,
        r,
        t,
        y,
        u,
        i,
        o,
        p,
        left_brace,
        right_brace,
        enter,
        left_ctrl,
        a,
        s,
        d,
        f,
        g,
        h,
        j,
        k,
        l,
        semicolon,
        apostrophe,
        grave,
        left_shift,
        backslash,
        z,
        x,
        c,
        v,
        b,
        n,
        m,
        comma,
        dot,
        slash,
        right_shift,
        kp_asterisk,
        left_alt,
        space,
        capslock,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        num_lock,
        scroll_lock,
        kp_num_7,
        kp_num_8,
        kp_num_9,
        kp_minus,
        kp_num_4,
        kp_num_5,
        kp_num_6,
        kp_plus,
        kp_num_1,
        kp_num_2,
        kp_num_3,
        kp_num_0,
        kp_dot,
        key_102nd,
        f11,
        f12,
        ro,
        kp_jpcomma,
        kp_enter,
        right_ctrl,
        kp_slash,
        right_alt,
        home,
        up,
        page_up,
        left,
        right,
        end,
        down,
        page_down,
        insert,
        del,
        kp_equal,
        kp_plus_minus,
        pause,
        kp_comma,
        yen,
        left_meta,
        right_meta,
        menu,
        back,
        forward,
        next_song,
        play_pause,
        prev_song,
        record,
        refresh,
        scroll_up,
        scroll_down,
        kp_left_paren,
        kp_right_paren,
        f13,
        f14,
        f15,
        f16,
        f17,
        f18,
        f19,
        f20,
        f21,
        f22,
        f23,
        f24,
    };

    /**
     * @brief Payload of a keyboard key press/release event.
     */
    struct KeyboardKeyEvent {
        Key  key{ Key::invalid }; /**< Key that changed state. */
        bool pressed{ false };    /**< true if pressed, false if released. */
    };

    /**
     * @brief Payload of a keyboard text input event.
     */
    struct KeyboardTextEvent {
        std::string_view text; /**< Text that was inserted by the user. */
    };

    /**
     * @brief State of a keyboard.
     */
    struct Keyboard {
        std::array<bool, std::numeric_limits<u8>::max()> keys{};           /**< Current key states. */
        std::chrono::milliseconds                        repeat_delay{};   /**< Delay for starting a key-repeat event. */
        i32                                              repeat_rate{ 0 }; /**< Number of key repeats per second. */
    };
} // namespace rw