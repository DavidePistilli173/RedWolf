module;

#include "vendor/glfw.hpp"

export module redwolf.ui.common;

export namespace rw::ui {
    using WindowHandle = GLFWwindow*;                        /**< Raw window handle type. */
    constexpr WindowHandle invalid_window_handle{ nullptr }; /**< Invalid window handle. */
} // namespace rw::ui
