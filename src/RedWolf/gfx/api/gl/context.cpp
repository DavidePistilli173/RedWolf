module;

#include "vendor/glfw.hpp"

#include <glad/glad.h>

module redwolf.gfx.context;

import redwolf.util.logger;

rw::gfx::Context::Context(rw::ui::WindowHandle window) : window_{ window } {
    if (rw::ui::invalid_window_handle == window_) {
        rw::err("Invalid window handle.");
        return;
    }
}

bool rw::gfx::Context::init() {
    glfwMakeContextCurrent(window_);

    // Load OpenGL functions using GLAD
    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        rw::err("Failed to initialize GLAD: {}", rw::vendor::glfw_get_error());
        return false;
    }

    rw::info("Loaded OpenGL functions.");
    rw::info("Graphics driver:");
    rw::info("   Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    rw::info("   Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    rw::info("   Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    return true;
}

void rw::gfx::Context::swap_buffers() {
    glfwSwapBuffers(window_);
}