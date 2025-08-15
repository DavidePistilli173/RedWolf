//
// Created by cjm036653 on 15/08/2025.
//

#include "input.hpp"

#include "../engine/app.hpp"

#include <GLFW/glfw3.h>

bool rw::input::is_key_down(const int key) {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    const int   state{ glfwGetKey(window, key) };
    return GLFW_PRESS == state || GLFW_REPEAT == state;
}

bool rw::input::is_key_up(const int key) {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    return GLFW_RELEASE == glfwGetKey(window, key);
}

bool rw::input::is_mouse_button_down(const int button) {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    return GLFW_PRESS == glfwGetMouseButton(window, button);
}

bool rw::input::is_mouse_button_up(const int button) {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    return GLFW_RELEASE == glfwGetMouseButton(window, button);
}

rw::core::Point2D<double> rw::input::mouse_pos() {
    GLFWwindow*       window{ rw::engine::App::get().window().handle() };
    rw::core::Point2D position;
    glfwGetCursorPos(window, &position.x, &position.y);
    return position;
}

double rw::input::mouse_x() {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    double      x{ 0.0 };
    glfwGetCursorPos(window, &x, nullptr);
    return x;
}

double rw::input::mouse_y() {
    GLFWwindow* window{ rw::engine::App::get().window().handle() };
    double      y{ 0.0 };
    glfwGetCursorPos(window, nullptr, &y);
    return y;
}