#pragma once

#include "common.hpp"

namespace rw {
    /**
     * @brief Rectangle.
     */
    struct Rect {
        f32 x{ 0.0F }; /**< X coordinate of the top-left corner. */
        f32 y{ 0.0F }; /**< Y coordinate of the top-left corner. */
        f32 w{ 0.0F }; /**< Width of the rectangle. */
        f32 h{ 0.0F }; /**< Height of the rectangle. */
    };
} // namespace rw