#pragma once

#include "common.hpp"

namespace rw {
    /**
     * @brief Colour.
     */
    struct Color {
        f32 r{ 0.0F }; /**< Red component. */
        f32 g{ 0.0F }; /**< Green component. */
        f32 b{ 0.0F }; /**< Blue component. */
        f32 a{ 0.0F }; /**< Alpha component. */
    };
} // namespace rw