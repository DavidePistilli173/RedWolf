#include "RedWolf/gl/Shader.hpp"

#include "RedWolf/Log.hpp"
#include "RedWolf/io/Parser.hpp"
#include "RedWolf/io/Text.hpp"
#include "RedWolf/utility.hpp"

#include <array>

namespace rw::gl
{
   Shader::Shader()
   {
      id_ = glCreateProgram();
   }

   /**
      \param code Files containing the vertex and fragment shader respectively.
   */
   Shader::Shader(std::pair<std::string_view, std::string_view> code)
   {
      // Load the source code for the shaders.
      std::string vertexCode{ rw::io::Text::load(code.first) };
      if (vertexCode.empty())
      {
         Log::core_err("Empty vertex shader: \"{1}\".", code.first);
         throw std::exception{};
      }
      std::string fragmentCode{ rw::io::Text::load(code.second) };
      if (fragmentCode.empty())
      {
         Log::core_err("Empty fragment shader: \"{1}\".", code.second);
         throw std::exception{};
      }

      // Create the vertex shader.
      GLuint        vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
      const GLchar* vertexShaderSource{ vertexCode.c_str() };
      glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

      // Create the fragment shader.
      GLuint        fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
      const GLchar* fragmentShaderSource{ fragmentCode.c_str() };
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

      GLint                      status;
      std::array<char, log_size> log;

      // Compile the vertex shader.
      glCompileShader(vertexShader);
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
      if (status == 0)
      {
         glGetShaderInfoLog(vertexShader, log_size, nullptr, log.data());
         glDeleteShader(vertexShader);
         glDeleteShader(fragmentShader);
         Log::core_err("Failed to compile vertex shader \"{1}\"\n   {2}", code.first, log.data());
         throw std::exception{};
      }

      // Compile the fragment shader.
      glCompileShader(fragmentShader);
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
      if (status == 0)
      {
         glGetShaderInfoLog(fragmentShader, log_size, nullptr, log.data());
         glDeleteShader(vertexShader);
         glDeleteShader(fragmentShader);
         Log::core_err("Failed to compile fragment shader \"{1}\"\n   {2}", code.second, log.data());
         throw std::exception{};
      }

      // Attach shaders and link the shader program.
      id_ = glCreateProgram();
      glAttachShader(id_, vertexShader);
      glAttachShader(id_, fragmentShader);
      glLinkProgram(id_);
      glGetProgramiv(id_, GL_LINK_STATUS, &status);
      if (status == 0)
      {
         glGetProgramInfoLog(id_, log_size, nullptr, log.data());
         glDeleteProgram(id_);
         glDeleteShader(vertexShader);
         glDeleteShader(fragmentShader);
         Log::core_err(
            "Failed to link shader from vertex shader \"{1}\" and fragment shader \"{2}\".\n   {3}",
            code.first,
            code.second,
            log.data());
         throw std::exception{};
      }

      // Extract uniform locations.
      use();
      auto extractUniformLocations = [this](const std::string& code) {
         using rw::io::Parser;
         Parser      codeParser{ code };
         std::string uniformName;
         auto        lines = codeParser.findAll(uniform_token);
         for (const auto& line : lines)
         {
            auto tokens = codeParser.tokeniseLine(Parser::glsl_ufrm_delims, Parser::glsl_ufrm_endln, line);
            uniformName = tokens[tokens.size() - 1];
            uniforms_.emplace_back(glGetUniformLocation(id_, uniformName.c_str()));
         }
      };

      // Extract uniforms from the vertex shader.
      extractUniformLocations(vertexCode);
      extractUniformLocations(fragmentCode);

      // Clean-up
      glDetachShader(id_, vertexShader);
      glDetachShader(id_, fragmentShader);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
   }

   Shader::Shader(Shader&& oldShader) noexcept
   {
      id_ = oldShader.id_;
      oldShader.id_ = 0;
      uniforms_ = std::move(oldShader.uniforms_);
   }

   Shader& Shader::operator=(Shader&& oldShader) noexcept
   {
      glDeleteProgram(id_);
      id_ = oldShader.id_;
      oldShader.id_ = 0;
      uniforms_ = std::move(oldShader.uniforms_);
      return *this;
   }

   Shader::~Shader()
   {
      glDeleteProgram(id_);
   }

   /**
      \param idx Index of the uniform to modify. 
      They are ordered by appearance, from vertex shader to fragment shader.
      \param i New uniform value.
   */
   void Shader::setUniform(size_t idx, int i)
   {
      if constexpr (debug)
      {
         if (!checkUniformIndex_(idx)) throw std::exception{};
      }
      glUniform1i(uniforms_[idx], i);
   }

   /**
      \param idx Index of the uniform to modify. 
      They are ordered by appearance, from vertex shader to fragment shader.
      \param mat New mat value.
   */
   void Shader::setUniform(size_t idx, const GLfloat* mat)
   {
      if constexpr (debug)
      {
         if (!checkUniformIndex_(idx)) throw std::exception{};
      }
      glUniformMatrix4fv(uniforms_[idx], 1, GL_FALSE, mat);
   }

   void Shader::use() const
   {
      glUseProgram(id_);
   }

   /**
      \param idx Index to check.
      \return True if the index is valid, false otherwise.
   */
   bool Shader::checkUniformIndex_(size_t idx)
   {
      if (idx >= uniforms_.size())
      {
         rw::Log::core_err("Uniform required: {}; max uniform present: {}.", idx, uniforms_.size() - 1);
         return false;
      }
      return true;
   }
} // namespace rw::gl