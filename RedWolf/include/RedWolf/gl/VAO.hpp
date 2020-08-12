#ifndef RW_GL_VAO_HPP
#define RW_GL_VAO_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/gl/Buffer.hpp"
#include "RedWolf/gl/Shader.hpp"

#include <glad/glad.h>

namespace rw::gl
{
   /** \brief Wrapper for a Vertex Array Object. 
       Only suppors vertices in 3D space and 2D textures.
   */
   template<size_t VBOSize, size_t EBOSize>
   class VAO
   {
   public:
      /** \brief Maximum size of the VBO. */
      static constexpr size_t max_vbo_size{ 100 };
      /** \brief Maximum size of the EBO. */
      static constexpr size_t max_ebo_size{ 100 };

      /** \brief Number of coordinate components for each vertex. 
          Forces the class to use 3D vertices.
      */
      static constexpr GLint vtx_coords_num{ 3 };

      /** \brief Number of texture coordinate components for each vertex. 
          Forces the class to use 2D textures.
      */
      static constexpr GLint tex_coords_num{ 2 };

      /** \brief Number of data elements for each vertex. */
      static constexpr GLint vbo_stride_elems{ vtx_coords_num + tex_coords_num };

      /** \brief Construct an empty VAO. */
      VAO();

      /** \brief Construct an empty VAO with custom usage guidelines. */
      VAO(GLenum usage);

      /** \brief Destructor. */
      ~VAO();

      /** \brief A VAO cannot be copied. */
      VAO(const VAO&) = delete;

      /** \brief Move constructor. */
      VAO(VAO&& oldVAO);

      /** \brief A VAO cannot be copied. */
      VAO& operator=(const VAO&) = delete;

      /** \brief Move assignment operator. */
      VAO& operator=(VAO&& oldVAO);

      /** \brief Bind the VAO. */
      void bind() const;

      /** \brief Set the EBO data directly.*/
      void setEBO(const std::array<unsigned int, EBOSize>& data);

      /** \brief Set a new EBO. */
      void setEBO(Buffer<unsigned int, EBOSize>&& buff);

      /** \brief Set the VBO data directly. Fastest option. */
      void setVBO(const std::array<float, VBOSize>& data);

      /** \brief Set a new VBO. Slowest option. */
      void setVBO(Buffer<float, VBOSize>&& buff);

   private:
      /** \brief Set the attribute locations and enable them. */
      void setAttributes_();

      GLuint                        id_{ 0 }; /**< OpenGL ID of the VAO. */
      Buffer<float, VBOSize>        vbo_{ GL_ARRAY_BUFFER, GL_STATIC_DRAW };
      Buffer<unsigned int, EBOSize> ebo_{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW };
   };

   /** \brief Supported VAO sizes. */
   constexpr std::array vao_sizes{
      std::pair{ 0, 0 },
      std::pair{ 5, 1 },
      std::pair{ 10, 2 },
      std::pair{ 15, 3 },
      std::pair{ 20, 6 },
      std::pair{ 25, 6 },
      std::pair{ 25, 9 },
      std::pair{ 30, 6 },
      std::pair{ 30, 9 },
      std::pair{ 30, 12 },
      std::pair{ 35, 6 },
      std::pair{ 35, 9 },
      std::pair{ 35, 12 },
      std::pair{ 35, 15 },
      std::pair{ 40, 6 },
      std::pair{ 40, 9 },
      std::pair{ 40, 12 },
      std::pair{ 40, 15 },
      std::pair{ 40, 18 },
   };
} // namespace rw::gl

template<size_t VBOSize, size_t EBOSize>
inline rw::gl::VAO<VBOSize, EBOSize>::VAO()
{
   // Generate the VAO.
   glGenVertexArrays(1, &id_);

   setAttributes_();
}

/**
   \param usage Expected usage of the VBO (static, dynamic or stream draws).
*/
template<size_t VBOSize, size_t EBOSize>
inline rw::gl::VAO<VBOSize, EBOSize>::VAO(GLenum usage)
{
   // Generate the VAO.
   glGenVertexArrays(1, &id_);

   vbo_ = rw::gl::Buffer<float, VBOSize>{ GL_ARRAY_BUFFER, usage };
   setAttributes_();
}

template<size_t VBOSize, size_t EBOSize>
inline rw::gl::VAO<VBOSize, EBOSize>::~VAO()
{
   glDeleteVertexArrays(1, &id_);
}

template<size_t VBOSize, size_t EBOSize>
inline rw::gl::VAO<VBOSize, EBOSize>::VAO(VAO&& oldVAO)
{
   id_ = oldVAO.id_;
   oldVAO.id_ = 0;

   vbo_ = std::move(oldVAO.vbo_);
   ebo_ = std::move(oldVAO.ebo_);

   setAttributes_();
}

template<size_t VBOSize, size_t EBOSize>
inline rw::gl::VAO<VBOSize, EBOSize>& rw::gl::VAO<VBOSize, EBOSize>::operator=(VAO&& oldVAO)
{
   if (this != &oldVAO)
   {
      glDeleteVertexArrays(1, &id_);

      id_ = oldVAO.id_;
      oldVAO.id_ = 0;

      vbo_ = std::move(oldVAO.vbo_);
      ebo_ = std::move(oldVAO.ebo_);

      setAttributes_();
   }
   return *this;
}

template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::bind() const
{
   glBindVertexArray(id_);
}

/**
   \param data Data to store in the EBO.
*/
template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::setEBO(const std::array<unsigned int, EBOSize>& data)
{
   ebo_.setData(data);
}

/**
   \param buff Source data buffer. It will be moved.
*/
template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::setEBO(rw::gl::Buffer<unsigned int, EBOSize>&& buff)
{
   ebo_ = std::move(buff);
   glBindVertexArray(id_);
   ebo_.bind();
}

/**
   \param data Data to store in the VBO.
*/
template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::setVBO(const std::array<float, VBOSize>& data)
{
   vbo_.setData(data);
}

/**
   \param buff Source data buffer. It will be moved.
*/
template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::setVBO(rw::gl::Buffer<float, VBOSize>&& buff)
{
   vbo_ = std::move(buff);
   setAttributes_();
}

template<size_t VBOSize, size_t EBOSize>
inline void rw::gl::VAO<VBOSize, EBOSize>::setAttributes_()
{
   // Bind the buffers.
   glBindVertexArray(id_);
   vbo_.bind();
   ebo_.bind();

   // Set the attributes.
   using Attrib = rw::gl::Shader::Attrib;
   GLsizei strideSize{ vbo_stride_elems * sizeof(float) };

   glVertexAttribPointer(
      static_cast<GLuint>(Attrib::vtx_coords),
      vtx_coords_num,
      GL_FLOAT,
      GL_FALSE,
      strideSize,
      nullptr);
   glVertexAttribPointer(
      static_cast<GLuint>(Attrib::tex_coords),
      tex_coords_num,
      GL_FLOAT,
      GL_FALSE,
      strideSize,
      reinterpret_cast<void*>(vtx_coords_num * sizeof(float)));

   // Enable the attributes.
   glEnableVertexAttribArray(static_cast<GLuint>(Attrib::vtx_coords));
   glEnableVertexAttribArray(static_cast<GLuint>(Attrib::tex_coords));
}

// Template instantiations.
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[0].first, rw::gl::vao_sizes[0].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[1].first, rw::gl::vao_sizes[1].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[2].first, rw::gl::vao_sizes[2].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[3].first, rw::gl::vao_sizes[3].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[4].first, rw::gl::vao_sizes[4].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[5].first, rw::gl::vao_sizes[5].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[6].first, rw::gl::vao_sizes[6].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[7].first, rw::gl::vao_sizes[7].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[8].first, rw::gl::vao_sizes[8].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[9].first, rw::gl::vao_sizes[9].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[10].first, rw::gl::vao_sizes[10].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[11].first, rw::gl::vao_sizes[11].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[12].first, rw::gl::vao_sizes[12].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[13].first, rw::gl::vao_sizes[13].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[14].first, rw::gl::vao_sizes[14].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[15].first, rw::gl::vao_sizes[15].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[16].first, rw::gl::vao_sizes[16].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[17].first, rw::gl::vao_sizes[17].second>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[18].first, rw::gl::vao_sizes[18].second>;

#endif