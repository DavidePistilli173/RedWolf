//
// Created by cjm036653 on 19/08/2025.
//

#ifndef SRC_REDWOLF_GL_SHADER_HPP
#define SRC_REDWOLF_GL_SHADER_HPP

#include "RedWolf/core/asset_library.hpp"
#include "RedWolf/math/math.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace rw::gfx {
    class ShaderLibrary;
}

namespace rw::gfx::api::gl {
    /**
     * @brief OpenGL shader class.
     */
    class Shader {
     public:
        /**
         * @brief Convert a string to a GLenum shader type.
         * @param type String representing the shader type.
         * @return GLenum corresponding to the shader type, or nothing if the type is invalid.
         */
        static constexpr std::optional<GLenum> shader_type_from_string(const std::string_view type) {
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
         * @brief Create a shader by providing the shader code through strings.
         * @param vertex_src Vertex shader source code.
         * @param fragment_src Fragment shader source code.
         */
        Shader(const std::string& vertex_src, const std::string& fragment_src);

        /**
         * @brief Create a shader by providing the file path to the shader code.
         * @param file_path File path to the shader code.
         */
        explicit Shader(const std::string& file_path);

        /**
         * @brief Destructor.
         */
        ~Shader();

        /**
         * @brief Copy constructor.
         */
        Shader(const Shader&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Shader& operator=(const Shader&) = delete;

        /**
         * @brief Move constructor.
         */
        Shader(Shader&&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        Shader& operator=(Shader&&) = delete;

        /**
         * @brief Bind the shader program.
         */
        void bind() const;

        /**
         * @brief Set a 1 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param value Float to set.
         */
        void set_f32(const std::string_view name, const float value) const;

        /**
         * @brief Set a 2 component float uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_2(const std::string_view name, const rw::math::Vec2& vec) const;

        /**
         * @brief Set a 3 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_3(const std::string_view name, const rw::math::Vec3& vec) const;

        /**
         * @brief Set a 4 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to set.
         */
        void set_f32_4(const std::string_view name, const rw::math::Vec4& vec) const;

        /**
         * @brief Set a 1 component int32 uniform to the shader.
         * @param name Name of the uniform.
         * @param value Int to set.
         */
        void set_i32(const std::string_view name, const int32_t value) const;

        /**
         * @brief Set a 3x3 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to set.
         */
        void set_mat_f32_3(const std::string_view name, const rw::math::Mat3& matrix) const;

        /**
         * @brief Set a 4x4 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to set.
         */
        void set_mat_f32_4(const std::string_view name, const rw::math::Mat4& matrix) const;

        /**
         * @brief Unbind the shader program.
         */
        void unbind() const;

     private:
        /**
         * @brief Compile the shader sources.
         * @param shader_sources Shader sources to compile.
         */
        void compile_(const std::unordered_map<GLenum, std::string>& shader_sources);

        /**
         * @brief Parse the shader source code and separate it into different shader types.
         * @param source Shader source code to parse.
         * @return Map of all the shader types found in the source code.
         */
        [[nodiscard]] static std::unordered_map<GLenum, std::string> pre_process_(const std::string& source);

        /**
         * @brief Load a file and return its contents as a string.
         * @param path Path of the file to read.
         * @return File contents.
         */
        [[nodiscard]] static std::string read_file_(const std::string& path);

        /**
         * @brief Upload a 1 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param value Float to upload.
         */
        void upload_uniform_f32_(const std::string_view name, const float value) const;

        /**
         * @brief Upload a 2 component float uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_2_(const std::string_view name, const rw::math::Vec2& vec) const;

        /**
         * @brief Upload a 3 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_3_(const std::string_view name, const rw::math::Vec3& vec) const;

        /**
         * @brief Upload a 4 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_4_(const std::string_view name, const rw::math::Vec4& vec) const;

        /**
         * @brief Upload a 1 component int32 uniform to the shader.
         * @param name Name of the uniform.
         * @param value Int to upload.
         */
        void upload_uniform_i32_(const std::string_view name, const int32_t value) const;

        /**
         * @brief Upload a 3x3 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void upload_uniform_mat_f32_3_(const std::string_view name, const rw::math::Mat3& matrix) const;

        /**
         * @brief Upload a 4x4 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void upload_uniform_mat_f32_4_(const std::string_view name, const rw::math::Mat4& matrix) const;

        uint32_t id_{ 0U }; /**< ID of the shader. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_SHADER_HPP
