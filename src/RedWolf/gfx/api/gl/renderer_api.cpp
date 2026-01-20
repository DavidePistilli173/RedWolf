module;

#include "Redwolf/macros.hpp"

#include <glad/glad.h>
#include <string>

module redwolf.gfx.renderer_api;

import redwolf.gfx.vertex_array;
import redwolf.math;
import redwolf.util.logger;

static void error_callback(
    GLenum                       source,
    GLenum                       type,
    [[maybe_unused]] GLuint      id,
    GLenum                       severity,
    [[maybe_unused]] GLsizei     length,
    const GLchar*                message,
    [[maybe_unused]] const void* userParam) {
    std::string source_str;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        source_str = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "Other";
        break;
    default:
        source_str = "Unknown";
        break;
    }

    std::string type_str;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        type_str = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_str = "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        type_str = "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "Other";
        break;
    default:
        type_str = "Unknown";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        RW_CORE_ERR("OpenGL: source:{}, type: {}, message: {}", source_str, type_str, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        RW_CORE_WARN("OpenGL: source:{}, type: {}, message: {}", source_str, type_str, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        RW_CORE_INFO("OpenGL: source:{}, type: {}, message: {}", source_str, type_str, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        // Ignore notifications because they are noisy.
        // RW_CORE_TRACE("OpenGL: source:{}, type: {}, message: {}", source_str, type_str, message);
        break;
    default:
        RW_CORE_FATAL("OpenGL: source:{}, type: {}, severity: {}, message: {}", source_str, type_str, severity, message);
        break;
    }
}

void rw::gfx::RendererApi::clear_screen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rw::gfx::RendererApi::draw_indexed(const VertexArray* vertex_array, const uint32_t index_count) {
    if (0 == index_count) {
        glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<int>(index_count), GL_UNSIGNED_INT, nullptr);
    }
}

void rw::gfx::RendererApi::init() {
    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&error_callback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void rw::gfx::RendererApi::set_clear_color(const rw::math::Vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void rw::gfx::RendererApi::set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height) {
    glViewport(x, y, width, height);
}
