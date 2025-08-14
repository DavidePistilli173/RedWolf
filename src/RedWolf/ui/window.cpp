//
// Created by cjm036653 on 13/08/25.
//

#include "window.hpp"

#include "../evt/application_event.hpp"
#include "../evt/key_event.hpp"
#include "../evt/mouse_event.hpp"
#include "../util/logger.hpp"

#include <glad/glad.h>

bool rw::ui::Window::glfw_initialized_{ false };

rw::ui::Window::Window(const WindowDescriptor& descriptor) :
    title_{ descriptor.title }, width_{ descriptor.width }, height_{ descriptor.height } {
    // Initialize GLFW
    if (!glfw_initialized_) {
        if (GLFW_TRUE != glfwInit()) {
            RW_CORE_FATAL("Failed to initialise GLFW: {}", rw::vendor::glfw_get_error());
            return;
        }

        glfwSetErrorCallback(glfw_error_clbk_);
        RW_CORE_INFO("GLFW initialised successfully");
        glfw_initialized_ = true;
    }

    // Create the window
    handle_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str(), nullptr, nullptr);
    if (nullptr == handle_) {
        RW_CORE_ERR("Failed to create window: {}", rw::vendor::glfw_get_error());
        return;
    }

    glfwMakeContextCurrent(handle_);

    // Load OpenGL functions using GLAD
    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        RW_CORE_ERR("Failed to initialize GLAD: {}", rw::vendor::glfw_get_error());
        return;
    }
    RW_CORE_INFO("Loaded OpenGL functions.");

    glfwSetWindowUserPointer(handle_, this);
    set_vsync(true);

    init_callbacks_();
}

rw::ui::Window::~Window() {
    close_();
}

rw::ui::Window::Window(Window&& other) noexcept :
    handle_{ other.handle_ }, title_{ std::move(other.title_) }, width_{ other.width_ }, height_{ other.height_ }, vsync_{ other.vsync_ },
    event_callback_{ std::move(other.event_callback_) } {
    other.handle_ = nullptr; // Transfer ownership
}

uint32_t rw::ui::Window::height() const {
    return height_;
}

void rw::ui::Window::update() {
    glfwWaitEventsTimeout(default_frame_time);
    glfwSwapBuffers(handle_);
}

void rw::ui::Window::set_event_callback(const std::function<bool(const rw::evt::Event&)>& callback) {
    event_callback_ = callback;
}

void rw::ui::Window::set_vsync(const bool enabled) {
    vsync_ = enabled;

    if (vsync_) {
        glfwSwapInterval(1); // Enable VSync
        RW_CORE_INFO("VSync enabled");
    } else {
        glfwSwapInterval(0); // Disable VSync
        RW_CORE_INFO("VSync disabled");
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

void rw::ui::Window::cursor_position_clbk_(GLFWwindow* window, double x, double y) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    const auto* self{ static_cast<Window*>(user_ptr) };

    const rw::evt::MouseMovedEvent event{ x, y };
    if (nullptr != self->event_callback_) {
        (void) self->event_callback_(event);
    }
}

void rw::ui::Window::glfw_error_clbk_(int code, const char* description) {
    RW_CORE_ERR("GLFW error: code: {}; description: {}", static_cast<rw::vendor::GlfwError>(code), description);
}

void rw::ui::Window::init_callbacks_() {
    glfwSetWindowCloseCallback(handle_, &Window::window_close_clbk_);
    glfwSetWindowSizeCallback(handle_, &Window::window_resize_clbk_);
    glfwSetKeyCallback(handle_, &Window::key_clbk_);
    glfwSetMouseButtonCallback(handle_, &Window::mouse_button_clbk_);
    glfwSetScrollCallback(handle_, &Window::scroll_clbk_);
    glfwSetCursorPosCallback(handle_, &Window::cursor_position_clbk_);
}

void rw::ui::Window::key_clbk_(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    const auto* self{ static_cast<Window*>(user_ptr) };

    switch (action) {
    case GLFW_PRESS: {
        const rw::evt::KeyPressedEvent event{ key, false };
        if (nullptr != self->event_callback_) {
            self->event_callback_(event);
        }
    } break;
    case GLFW_RELEASE: {
        const rw::evt::KeyReleasedEvent event{ key };
        if (nullptr != self->event_callback_) {
            self->event_callback_(event);
        }
    } break;
    case GLFW_REPEAT: {
        const rw::evt::KeyPressedEvent event{ key, true };
        if (nullptr != self->event_callback_) {
            self->event_callback_(event);
        }
    } break;
    default:
        RW_CORE_ERR("Invalid GLFW action: {}", action);
        break;
    }
}

void rw::ui::Window::mouse_button_clbk_(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    const auto* self{ static_cast<Window*>(user_ptr) };

    switch (action) {
    case GLFW_PRESS: {
        const rw::evt::MouseButtonPressedEvent event{ button };
        if (nullptr != self->event_callback_) {
            self->event_callback_(event);
        }
    } break;
    case GLFW_RELEASE: {
        const rw::evt::MouseButtonReleasedEvent event{ button };
        if (nullptr != self->event_callback_) {
            self->event_callback_(event);
        }
    } break;
    default:
        RW_CORE_ERR("Invalid GLFW action: {}", action);
        break;
    }
}

void rw::ui::Window::scroll_clbk_(GLFWwindow* window, double x_offset, double y_offset) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    auto* self{ static_cast<Window*>(user_ptr) };

    const rw::evt::MouseScrolledEvent event{ x_offset, y_offset };
    if (nullptr != self->event_callback_) {
        self->event_callback_(event);
    }
}

void rw::ui::Window::window_close_clbk_(GLFWwindow* window) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    auto* self{ static_cast<Window*>(user_ptr) };

    const rw::evt::WindowCloseEvent event{};
    if (nullptr != self->event_callback_) {
        self->event_callback_(event);
    }
}

void rw::ui::Window::window_resize_clbk_(GLFWwindow* window, int width, int height) {
    void* user_ptr{ glfwGetWindowUserPointer(window) };
    if (nullptr == user_ptr) {
        return;
    }
    auto* self{ static_cast<Window*>(user_ptr) };

    if (width <= 0 || height <= 0) {
        RW_CORE_WARN("Invalid window size: {}x{}", width, height);
        return;
    }
    self->width_  = static_cast<uint32_t>(width);
    self->height_ = static_cast<uint32_t>(height);

    if (nullptr == self->event_callback_) {
        return;
    }
    rw::evt::WindowResizeEvent event{ self->width_, self->height_ };
    self->event_callback_(event);
}
