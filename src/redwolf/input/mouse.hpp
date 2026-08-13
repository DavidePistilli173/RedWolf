#pragma once

#include "redwolf/common.hpp"

#include <array>
#include <optional>
#include <utility>

namespace rw {
    /**
     * @brief Mouse buttons.
     */
    enum class MouseBtn : u8 { invalid, left, right, middle, side, extra, forward, back, task };
    static constexpr usize mouse_btn_num{ 9 }; /**< Number of mouse buttons (including invalid). */

    /**
     * @brief Payload of a mouse button event.
     */
    struct MouseBtnEvent {
        MouseBtn btn{ MouseBtn::invalid }; /**< Button that changed its state. */
        bool     pressed{ false };         /**< true if pressed, false if released. */
    };

    /**
     * @brief Payload of a mouse movement event.
     */
    struct MouseMoveEvent {
        f32 x{ 0.0F }; /**< New X coordinate (from the top-left corner). */
        f32 y{ 0.0F }; /**< New Y coordinate (from the top-left corner). */
    };

    /**
     * @brief Payload of a mouse scroll event.
     */
    struct MouseScrollEvent {
        f32 x{ 0.0F }; /**< Horizontal scrolling, normalized in [-1,1]. */
        f32 y{ 0.0F }; /**< Vertical scrolling, normalized in [-1,1]. */
    };

    /**
     * @brief Input state of a mouse.
     */
    struct Mouse {
        std::array<bool, mouse_btn_num> buttons{}; /**< Current button states: true=pressed; false=released. */
        f32                             x{ 0.0F }; /**< Current X coordinate (from the top-left corner of the window) */
        f32                             y{ 0.0F }; /**< Current Y coordinate (from the top-left corner of the window) */
    };
} // namespace rw