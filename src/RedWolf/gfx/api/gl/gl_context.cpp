//
// Created by cjm036653 on 18/08/2025.
//

#include "gl_context.hpp"

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
    RW_CORE_INFO("Graphics driver:");
    RW_CORE_INFO("   Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    RW_CORE_INFO("   Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    RW_CORE_INFO("   Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    return true;
}

void rw::gfx::api::gl::Context::swap_buffers() {
    glfwSwapBuffers(window_);
}