#pragma once

#include "redwolf/common.hpp"

namespace rw {
    /**
     * @brief Payload of a window close event.
     */
    struct WindowCloseEvent {};

    /**
     * @brief Payload of a window resize event.
     */
    struct WindowResizeEvent {
        u32 old_width{ 0U };  /**< Width before the resize. */
        u32 old_height{ 0U }; /**< Height before the resize. */
        u32 new_width{ 0U };  /**< Width after the resize. */
        u32 new_height{ 0U }; /**< Height after the resize. */
    };
} // namespace rw