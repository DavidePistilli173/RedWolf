//
// Created by cjm036653 on 18/08/2025.
//

#include "Context.hpp"

#include "RedWolf/util/logger.hpp"
#include "vendor/glfw.hpp"

#include <glad/glad.h>

rw::gfx::api::gl::Context::Context(rw::ui::WindowHandle window) : window_{ window } {
    if (rw::ui::invalid_window_handle == window_) {
        RW_CORE_ERR("Invalid window handle.");
        return;
    }
}

bool rw::gfx::api::gl::Context::init() {
    glfwMakeContextCurrent(window_);

    // Load OpenGL functions using GLAD
    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        RW_CORE_ERR("Failed to initialize GLAD: {}", rw::vendor::glfw_get_error());
        return false;
    }
    RW_CORE_INFO("Loaded OpenGL functions.");
    return true;
}

void rw::gfx::api::gl::Context::swap_buffers() {
    glfwSwapBuffers(window_);
}