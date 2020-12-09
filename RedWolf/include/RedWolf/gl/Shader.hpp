#ifndef RW_GL_SHADER_HPP
#define RW_GL_SHADER_HPP

#include "RedWolf/core.hpp"

#include <glad/glad.h>
#include <string_view>
#include <utility>
#include <vector>

namespace rw::gl
{
   /**< \brief Wrapper class for an OpenGL shader. */
   class RW_API Shader
   {
   public:
      /** \brief Positions of all different shader attributes. */
      enum class Attrib
      {
         vtx_coords, // Vertex coordinates.
         tex_coords  // Texture coordinates.
      };

      /** \brief Size of the error log for shader compilation and linking. */
      static constexpr size_t log_size{ 2048 };
      /** \brief Token for uniform parsing in shader code. */
      static constexpr std::string_view uniform_token{ "uniform" };

      /** \brief Create an empty shader. */
      Shader();

      /** \brief Create a shader from two source files. */
      explicit Shader(std::pair<std::string_view, std::string_view> code);

      /** \brief A shader cannot be copied. */
      Shader(const Shader&) = delete;

      /** \brief Move constructor. */
      Shader(Shader&& oldShader) noexcept;

      /** \brief A shader cannot be copied. */
      Shader& operator=(const Shader&) = delete;

      /** \brief Move assignment operator. */
      Shader& operator=(Shader&& oldShader) noexcept;

      /** \brief Destructor. */
      ~Shader();

      /** \brief Set the value of an int uniform. Requires that the shader is in use. */
      void setUniform(size_t idx, int i);

      /** \brief Set the value of a Mat4 uniform. Requires that the shader is in use. */
      void setUniform(size_t idx, const GLfloat* mat);

      /** \brief Use the shader for rendering. */
      void use() const;

   private:
      /** \brief Check if idx is a valid uniform index. */
      bool checkUniformIndex_(size_t idx);

      GLuint             id_{ 0 };  /**< OpenGL ID of the shader. */
      std::vector<GLint> uniforms_; /**< Uniform locations. */
   };
} // namespace rw::gl

#endif