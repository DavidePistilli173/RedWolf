//
// Created by cjm036653 on 19/08/2025.
//

#ifndef SRC_REDWOLF_GL_SHADER_HPP
#define SRC_REDWOLF_GL_SHADER_HPP

#include "RedWolf/math/math.hpp"

#include <cstdint>
#include <string_view>

namespace rw::gfx::api::gl {
    /**
     * @brief OpenGL shader class.
     */
    class Shader {
     public:
        /**
         * @brief Constructor.
         * @param vertex_src Vertex shader source code.
         * @param fragment_src Fragment shader source code.
         */
        Shader(std::string_view vertex_src, std::string_view fragment_src);

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
         * @brief Unbind the shader program.
         */
        void unbind() const;

        /**
         * @brief Upload a 1 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param value Float to upload.
         */
        void upload_uniform_f32(const std::string_view name, const float value) const;

        /**
         * @brief Upload a 2 component float uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_2(const std::string_view name, const rw::math::Vec2& vec) const;

        /**
         * @brief Upload a 3 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_3(const std::string_view name, const rw::math::Vec3& vec) const;

        /**
         * @brief Upload a 4 component float vector uniform to the shader.
         * @param name Name of the uniform.
         * @param vec Vector to upload.
         */
        void upload_uniform_f32_4(const std::string_view name, const rw::math::Vec4& vec) const;

        /**
         * @brief Upload a 1 component int32 uniform to the shader.
         * @param name Name of the uniform.
         * @param value Int to upload.
         */
        void upload_uniform_i32(const std::string_view name, const int32_t value) const;

        /**
         * @brief Upload a 3x3 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void upload_uniform_mat_f32_3(const std::string_view name, const rw::math::Mat3& matrix) const;

        /**
         * @brief Upload a 4x4 float matrix uniform to the shader.
         * @param name Name of the uniform.
         * @param matrix Matrix to upload.
         */
        void upload_uniform_mat_f32_4(const std::string_view name, const rw::math::Mat4& matrix) const;

     private:
        uint32_t id_{ 0U }; /**< ID of the shader. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_SHADER_HPP
