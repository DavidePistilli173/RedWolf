//
// Created by cjm036653 on 18/08/2025.
//

#ifndef SRC_REDWOLF_UI_HPP
#define SRC_REDWOLF_UI_HPP

#include "vendor/glfw.hpp"

namespace rw::ui {
    using WindowHandle = GLFWwindow*;                               /**< Raw window handle type. */
    static constexpr WindowHandle invalid_window_handle{ nullptr }; /**< Invalid window handle. */
} // namespace rw::ui

#endif // SRC_REDWOLF_UI_HPP
