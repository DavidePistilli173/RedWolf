//
// Created by cjm036653 on 19/08/2025.
//

#include "shader.hpp"

#include "RedWolf/util/logger.hpp"

#include <glad/glad.h>
#include <vector>

rw::gfx::api::gl::Shader::Shader(std::string_view vertex_src, std::string_view fragment_src) {
    // Create an empty vertex shader handle
    const GLuint vertex_shader{ glCreateShader(GL_VERTEX_SHADER) };

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const auto* source{ reinterpret_cast<const GLchar*>(vertex_src.data()) };
    glShaderSource(vertex_shader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertex_shader);

    GLint is_compiled{ 0 };
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
    if (GL_FALSE == is_compiled) {
        GLint max_len{ 0 };
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_len);

        // The max_len includes the NULL character
        std::vector<GLchar> info_log(static_cast<size_t>(max_len));
        glGetShaderInfoLog(vertex_shader, max_len, &max_len, info_log.data());

        // We don't need the shader anymore.
        glDeleteShader(vertex_shader);

        RW_CORE_ERR("Failed to compile vertex shader: {}", info_log.data());
        return;
    }

    // Create an empty fragment shader handle
    const GLuint fragment_shader{ glCreateShader(GL_FRAGMENT_SHADER) };

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = reinterpret_cast<const GLchar*>(fragment_src.data());
    glShaderSource(fragment_shader, 1, &source, nullptr);

    // Compile the fragment shader
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
    if (GL_FALSE == is_compiled) {
        GLint max_len{ 0 };
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_len);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(static_cast<size_t>(max_len));
        glGetShaderInfoLog(fragment_shader, max_len, &max_len, info_log.data());

        // We don't need the shader anymore.
        glDeleteShader(fragment_shader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertex_shader);

        RW_CORE_ERR("Failed to compile fragment shader: {}", info_log.data());
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    const GLuint program{ glCreateProgram() };

    // Attach our shaders to our program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint is_linked{ 0 };
    glGetProgramiv(program, GL_LINK_STATUS, reinterpret_cast<int*>(&is_linked));
    if (GL_FALSE == is_linked) {
        GLint max_len{ 0 };
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_len);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(static_cast<size_t>(max_len));
        glGetProgramInfoLog(program, max_len, &max_len, info_log.data());

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        RW_CORE_ERR("Failed to link shader: {}", info_log.data());
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    id_ = program;
    RW_CORE_INFO("Shader program created with ID: {}", id_);
}

rw::gfx::api::gl::Shader::~Shader() {
    glDeleteProgram(id_);
}

void rw::gfx::api::gl::Shader::bind() const {
    glUseProgram(id_);
}

void rw::gfx::api::gl::Shader::unbind() const {
    glUseProgram(0);
}