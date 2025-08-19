//
// Created by cjm036653 on 19/08/2025.
//

#ifndef SRC_REDWOLF_SHADER_HPP
#define SRC_REDWOLF_SHADER_HPP

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

     private:
        uint32_t id_{ 0U }; /**< ID of the shader. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_SHADER_HPP
