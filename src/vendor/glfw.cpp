//
// Created by cjm036653 on 13/08/25.
//

#include "glfw.hpp"

rw::vendor::GlfwError rw::vendor::glfw_get_error() {
    return static_cast<GlfwError>(glfwGetError(nullptr));
}