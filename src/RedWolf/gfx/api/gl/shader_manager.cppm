module;

#include "RedWolf/macros.hpp"

#include <fstream>
#include <glad/glad.h>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

export module redwolf.gfx.shader_manager;

import redwolf.common;
import redwolf.core.asset_library;
import redwolf.gfx.shader;
import redwolf.math;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class for managing shaders.
     */
    class ShaderManager {
     public:
        /**
         * @brief Constructor.
         */
        ShaderManager() = default;

        /**
         * @brief Destructor.
         */
        ~ShaderManager() {
            shaders_.remove_all([](Shader& shader) {
                glDeleteProgram(shader.gid);
                shader.gid = 0;
            });

            RW_CORE_TRACE("All shaders unloaded.");
        }

        /**
         * @brief Copy constructor.
         */
        ShaderManager(const ShaderManager&) = delete;

        /**
         * @brief Move constructor.
         */
        ShaderManager(ShaderManager&&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        ShaderManager& operator=(const ShaderManager&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        ShaderManager& operator=(ShaderManager&&) = delete;

        /**
         * @brief Bind a shader program.
         * @param shader_handle Handle to the shader to bind.
         */
        void bind(const Handle<Shader> shader_handle) const {
            shaders_.visit<void>(shader_handle, [](const Shader& shader) { glUseProgram(shader.gid); });
        }

        /**
         * @brief Create a shader by providing the file path to the shader code.
         * @param path File path to the shader code.
         */
        [[nodiscard]] Handle<Shader> new_from_path(const std::string_view path) {
            const auto shader_sources{ pre_process_(read_file_(path)) };
            const auto result{ compile_(shader_sources) };
            RW_CORE_TRACE("Shader (id:{}) created from {}.", result.index, path);
            return result;
        }

        /**
         * @brief Set a 1 component float vector uniform to a shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param value Float to set.
         */
        void set_f32(const Handle<Shader> shader_handle, const std::string_view name, const float value) const {
            upload_uniform_f32_(shader_handle, name, value);
        }

        /**
         * @brief Set a 2 component float uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_2(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec2& vec) const {
            upload_uniform_f32_2_(shader_handle, name, vec);
        }

        /**
         * @brief Set a 3 component float vector uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_3(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec3& vec) const {
            upload_uniform_f32_3_(shader_handle, name, vec);
        }

        /**
         * @brief Set a 4 component float vector uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_4(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec4& vec) const {
            upload_uniform_f32_4_(shader_handle, name, vec);
        }

        /**
         * @brief Set a 1 component int32 uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param value Int to set.
         */
        void set_i32(const Handle<Shader> shader_handle, const std::string_view name, const int32_t value) const {
            upload_uniform_i32_(shader_handle, name, value);
        }

        /**
         * @brief Set an array of int32 values to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param values Values to set.
         */
        void set_i32_array(const Handle<Shader> shader_handle, const std::string_view name, const std::span<const int32_t> values) const {
            upload_uniform_i32_array_(shader_handle, name, values);
        }

        /**
         * @brief Set a 3x3 float matrix uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to set.
         */
        void set_mat_f32_3(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Mat3& matrix) const {
            upload_uniform_mat_f32_3_(shader_handle, name, matrix);
        }

        /**
         * @brief Set a 4x4 float matrix uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to set.
         */
        void set_mat_f32_4(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Mat4& matrix) const {
            upload_uniform_mat_f32_4_(shader_handle, name, matrix);
        }
        /**
         * @brief Unbind the currently bound shader.
         */
        void unbind() const {
            glUseProgram(0);
        }

        /**
         * @brief Unload a given shader.
         * @param handle Handle to the shader to unload.
         */
        void unload(const Handle<Shader> handle) {
            shaders_.remove(handle, [](Shader& shader) {
                glDeleteProgram(shader.gid);
                shader.gid = 0;
            });

            RW_CORE_TRACE("Shader {} unloaded.", handle.index);
        }

     private:
        /**
         * @brief Compile the shader sources.
         * @param shader_sources Shader sources to compile.
         * @return Handle to the newly created shader.
         */
        [[nodiscard]] Handle<Shader> compile_(const std::unordered_map<GLenum, std::string>& shader_sources) {
            const GLuint        program{ glCreateProgram() };
            std::vector<GLuint> shader_ids;
            shader_ids.reserve(shader_sources.size());

            // Compile all provided shaders into a complete program.
            for (const auto& shader_source : shader_sources) {
                const auto [type, source]{ shader_source };

                // Create an empty shader handle
                const GLuint shader{ glCreateShader(type) };

                // Send the shader source code to GL
                const auto* source_ptr{ source.data() };
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
                    return {};
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
                return {};
            }

            // Always detach shaders after a successful link.
            for (const auto shader_id : shader_ids) {
                glDetachShader(program, shader_id);
            }

            auto [shader, shader_handle] = shaders_.create();

            shader.gid = program;
            return shader_handle;
        }

        /**
         * @brief Parse the shader source code and separate it into different shader types.
         * @param source Shader source code to parse.
         * @return Map of all the shader types found in the source code.
         */
        [[nodiscard]] static std::unordered_map<GLenum, std::string> pre_process_(const std::string_view source) {
            static constexpr std::string_view       type_token{ "#type" };
            std::unordered_map<GLenum, std::string> result;

            size_t pos{ source.find(type_token, 0) };
            while (pos != std::string_view::npos) {
                const size_t eol{ source.find_first_of("\r\n", pos) };
                if (std::string_view::npos == eol) {
                    RW_CORE_ERR("Failed to pre-process shader: syntax error, no shader code after type specifier '{}'", type_token);
                    return {};
                }

                const size_t           begin{ pos + type_token.size() + 1 }; // Skip the token and the single space.
                const std::string_view type = source.substr(begin, eol - begin);
                const auto             shader_type{ shader_type_from_string_(type) };
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

        [[nodiscard]] static std::string read_file_(const std::string_view path) {
            std::ifstream in_stream{ path.data(), std::ios::in | std::ios::binary };
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

        /**
         * @brief Convert a string to a GLenum shader type.
         * @param type String representing the shader type.
         * @return GLenum corresponding to the shader type, or nothing if the type is invalid.
         */
        static constexpr std::optional<GLenum> shader_type_from_string_(const std::string_view type) {
            if ("vertex" == type) {
                return GL_VERTEX_SHADER;
            }

            if ("fragment" == type || "pixel" == type) {
                return GL_FRAGMENT_SHADER;
            }

            if ("geometry" == type) {
                return GL_GEOMETRY_SHADER;
            }

            if ("compute" == type) {
                return GL_COMPUTE_SHADER;
            }

            if ("tess_control" == type) {
                return GL_TESS_CONTROL_SHADER;
            }

            if ("tess_evaluation" == type) {
                return GL_TESS_EVALUATION_SHADER;
            }

            return {};
        }

        /**
         * @brief Upload a 1 component float vector uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param value Float to upload.
         */
        void upload_uniform_f32_(const Handle<Shader> shader_handle, const std::string_view name, const float value) const {
            shaders_.visit<void>(shader_handle, [name, value](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniform1f(location, value);
            });
        }

        /**
         * @brief Upload a 2 component float uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_2_(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec2& vec) const {
            shaders_.visit<void>(shader_handle, [name, &vec](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniform2f(location, vec.x, vec.y);
            });
        }

        /**
         * @brief Upload a 3 component float vector uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_3_(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec3& vec) const {
            shaders_.visit<void>(shader_handle, [name, &vec](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniform3f(location, vec.x, vec.y, vec.z);
            });
        }

        /**
         * @brief Upload a 4 component float vector uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_4_(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Vec4& vec) const {
            shaders_.visit<void>(shader_handle, [name, &vec](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
            });
        }

        /**
         * @brief Upload a 1 component int32 uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param value Int to upload.
         */
        void upload_uniform_i32_(const Handle<Shader> shader_handle, const std::string_view name, const int32_t value) const {
            shaders_.visit<void>(shader_handle, [name, value](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniform1i(location, value);
            });
        }

        /**
         * @brief Upload an array of int32 values to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param values Values to set.
         */
        void upload_uniform_i32_array_(
            const Handle<Shader> shader_handle, const std::string_view name, const std::span<const int32_t> values) const {
            shaders_.visit<void>(shader_handle, [name, values](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }
                glUniform1iv(location, static_cast<GLsizei>(values.size()), values.data());
            });
        }

        /**
         * @brief Upload a 3x3 float matrix uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void
            upload_uniform_mat_f32_3_(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Mat3& matrix) const {
            shaders_.visit<void>(shader_handle, [name, &matrix](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniformMatrix3fv(location, 1, GL_FALSE, rw::math::value_ptr(matrix));
            });
        }

        /**
         * @brief Upload a 4x4 float matrix uniform to the shader.
         * @param shader_handle Handle to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void
            upload_uniform_mat_f32_4_(const Handle<Shader> shader_handle, const std::string_view name, const rw::math::Mat4& matrix) const {
            shaders_.visit<void>(shader_handle, [name, &matrix](const Shader& shader) {
                const GLint location{ glGetUniformLocation(shader.gid, name.data()) };
                if (-1 == location) {
                    RW_CORE_ERR("Failed to get uniform location: {}", name);
                    return;
                }

                glUniformMatrix4fv(location, 1, GL_FALSE, rw::math::value_ptr(matrix));
            });
        }
        rw::core::AssetLibrary<Shader> shaders_; /**< Actual storage for shaders. */
    };
} // namespace rw::gfx