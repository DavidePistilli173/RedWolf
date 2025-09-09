//
// Created by cjm036653 on 19/08/2025.
//

#include "gl_shader.hpp"

#include "RedWolf/util/logger.hpp"

#include <fstream>
#include <glad/glad.h>
#include <vector>

rw::gfx::api::gl::Shader::Shader(const std::string& vertex_src, const std::string& fragment_src) {
    const std::unordered_map<GLenum, std::string> shader_sources{ { GL_VERTEX_SHADER, vertex_src }, { GL_FRAGMENT_SHADER, fragment_src } };
    compile_(shader_sources);
}

rw::gfx::api::gl::Shader::Shader(const std::string& file_path) {
    const auto shader_sources{ pre_process_(read_file_(file_path)) };
    compile_(shader_sources);
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

void rw::gfx::api::gl::Shader::upload_uniform_f32(const std::string_view name, const float value) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniform1f(location, value);
}

void rw::gfx::api::gl::Shader::upload_uniform_f32_2(const std::string_view name, const rw::math::Vec2& vec) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniform2f(location, vec.x, vec.y);
}

void rw::gfx::api::gl::Shader::upload_uniform_f32_3(const std::string_view name, const rw::math::Vec3& vec) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniform3f(location, vec.x, vec.y, vec.z);
}

void rw::gfx::api::gl::Shader::upload_uniform_f32_4(const std::string_view name, const rw::math::Vec4& vec) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void rw::gfx::api::gl::Shader::upload_uniform_i32(const std::string_view name, const int32_t value) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniform1i(location, value);
}

void rw::gfx::api::gl::Shader::upload_uniform_mat_f32_3(const std::string_view name, const rw::math::Mat3& matrix) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniformMatrix3fv(location, 1, GL_FALSE, rw::math::value_ptr(matrix));
}

void rw::gfx::api::gl::Shader::upload_uniform_mat_f32_4(const std::string_view name, const rw::math::Mat4& matrix) const {
    const GLint location{ glGetUniformLocation(id_, name.data()) };
    if (-1 == location) {
        RW_CORE_ERR("Failed to get uniform location: {}", name);
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, rw::math::value_ptr(matrix));
}

void rw::gfx::api::gl::Shader::compile_(const std::unordered_map<GLenum, std::string>& shader_sources) {
    const GLuint        program{ glCreateProgram() };
    std::vector<GLuint> shader_ids;
    shader_ids.reserve(shader_sources.size());

    // Compile all provided shaders into a complete program.
    for (const auto& shader_source : shader_sources) {
        const GLenum       type{ shader_source.first };
        const std::string& source{ shader_source.second };

        // Create an empty shader handle
        const GLuint shader{ glCreateShader(type) };

        // Send the shader source code to GL
        const auto* source_ptr{ source.c_str() };
        glShaderSource(shader, 1, &source_ptr, nullptr);

        // Compile the vertex shader
        glCompileShader(shader);

        GLint is_compiled{ 0 };
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if (GL_FALSE == is_compiled) {
            GLint max_len{ 0 };
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);

            // The max_len includes the NULL character
            std::vector<GLchar> info_log(static_cast<size_t>(max_len));
            glGetShaderInfoLog(shader, max_len, &max_len, info_log.data());

            // We don't need the shader anymore.
            for (const auto shader_id : shader_ids) {
                glDeleteShader(shader_id);
            }
            glDeleteShader(shader);
            glDeleteProgram(program);

            RW_CORE_ERR("Failed to compile shader {}: {}", type, info_log.data());
            return;
        }
        glAttachShader(program, shader);
        shader_ids.emplace_back(shader);
    }

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
        for (const auto shader_id : shader_ids) {
            glDeleteShader(shader_id);
        }
        glDeleteProgram(program);

        RW_CORE_ERR("Failed to link shader: {}", info_log.data());
        return;
    }

    // Always detach shaders after a successful link.
    for (const auto shader_id : shader_ids) {
        glDetachShader(program, shader_id);
    }

    id_ = program;
    RW_CORE_TRACE("Shader program created with ID: {}", id_);
}

std::unordered_map<GLenum, std::string> rw::gfx::api::gl::Shader::pre_process_(const std::string& source) {
    static constexpr std::string_view       type_token{ "#type" };
    std::unordered_map<GLenum, std::string> result;

    size_t pos{ source.find(type_token, 0) };
    while (pos != std::string::npos) {
        const size_t eol{ source.find_first_of("\r\n", pos) };
        if (std::string::npos == eol) {
            RW_CORE_ERR("Failed to pre-process shader: syntax error, no shader code after type specifier '{}'", type_token);
            return {};
        }

        const size_t      begin{ pos + type_token.size() + 1 }; // Skip the token and the single space.
        const std::string type = source.substr(begin, eol - begin);
        const auto        shader_type{ shader_type_from_string(type) };
        if (!shader_type.has_value()) {
            RW_CORE_ERR("Invalid shader type: {}", type);
            return {};
        }

        const size_t next_line_pos{ source.find_first_not_of("\r\n", eol) };
        pos                         = source.find(type_token, pos + type_token.size());
        result[shader_type.value()] = source.substr(next_line_pos, pos - next_line_pos);
    }

    return result;
}

std::string rw::gfx::api::gl::Shader::read_file_(const std::string& path) {
    std::ifstream in_stream{ path, std::ios::binary };
    if (!in_stream) {
        RW_CORE_ERR("Failed to open shader file: {}", path);
        return {};
    }

    std::string result;
    in_stream.seekg(0, std::ios::end);
    result.resize(in_stream.tellg());
    in_stream.seekg(0, std::ios::beg);
    in_stream.read(result.data(), result.size());
    return result;
}
