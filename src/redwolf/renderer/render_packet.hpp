#pragma once

#include "redwolf/common.hpp"

namespace rw {
    /**
     * @brief Information required to draw a frame.
     */
    struct RenderPacket {
        f32 delta_time{ 0.0F }; /**< Time since the last frame. [s] */
    };
} // namespace rw