//
// Created by cjm036653 on 13/08/25.
//

#include "window.hpp"

bool rw::ui::Window::glfw_initialized_{ false };

rw::ui::Window::Window(const WindowDescriptor& descriptor) :
    logger_{ descriptor.logger }, title_{ descriptor.title }, width_{ descriptor.width }, height_{ descriptor.height } {
    // Initialize GLFW
    if (!glfw_initialized_) {
        if (GLFW_TRUE != glfwInit()) {
            logger_.fatal("Failed to initialise GLFW: {}", rw::vendor::glfw_get_error());
            return;
        }

        logger_.info("GLFW initialised successfully");
        glfw_initialized_ = true;
    }

    // Create the window
    handle_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str(), nullptr, nullptr);
    if (nullptr == handle_) {
        logger_.err("Failed to create window: {}", rw::vendor::glfw_get_error());
        return;
    }

    glfwMakeContextCurrent(handle_);
    glfwSetWindowUserPointer(handle_, this);
    set_vsync(true);
}

rw::ui::Window::~Window() {
    close_();
}

rw::ui::Window::Window(Window&& other) noexcept :
    logger_{ other.logger_ }, handle_{ other.handle_ }, title_{ std::move(other.title_) }, width_{ other.width_ }, height_{ other.height_ },
    vsync_{ other.vsync_ }, event_callback_{ std::move(other.event_callback_) } {
    other.handle_ = nullptr; // Transfer ownership
}

uint32_t rw::ui::Window::height() const {
    return height_;
}

void rw::ui::Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(handle_);
}

void rw::ui::Window::set_event_callback(const std::function<void(const rw::evt::Event&)>& callback) {
    event_callback_ = callback;
}

void rw::ui::Window::set_vsync(const bool enabled) {
    vsync_ = enabled;

    if (vsync_) {
        glfwSwapInterval(1); // Enable VSync
        logger_.info("VSync enabled");
    } else {
        glfwSwapInterval(0); // Disable VSync
        logger_.info("VSync disabled");
    }
}

bool rw::ui::Window::vsync() const {
    return vsync_;
}

uint32_t rw::ui::Window::width() const {
    return width_;
}

void rw::ui::Window::close_() {
    if (nullptr != handle_) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
    }
}
