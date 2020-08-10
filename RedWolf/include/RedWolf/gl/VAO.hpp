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
   template<size_t S>
   class VAO
   {
   public:
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
      /** \brief Number of elements of the EBO. */
      static constexpr GLint ebo_size{ S / vbo_stride_elems };

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
      void setEBO(const std::array<unsigned int, ebo_size>& data);

      /** \brief Set a new EBO. */
      void setEBO(Buffer<unsigned int, ebo_size>&& buff);

      /** \brief Set the VBO data directly. Fastest option. */
      void setVBO(const std::array<float, S>& data);

      /** \brief Set a new VBO. Slowest option. */
      void setVBO(Buffer<float, S>&& buff);

   private:
      /** \brief Set the attribute locations and enable them. */
      void setAttributes_();

      GLuint                         id_{ 0 }; /**< OpenGL ID of the VAO. */
      Buffer<float, S>               vbo_{ GL_ARRAY_BUFFER, GL_STATIC_DRAW };
      Buffer<unsigned int, ebo_size> ebo_{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW };
   };

   /** \brief Supported VAO sizes. */
   constexpr std::array vao_sizes{
      0, 1, 2, 3, 4, 6, 12, 20, 60, 64
   };
} // namespace rw::gl

template<size_t S>
inline rw::gl::VAO<S>::VAO()
{
   // Generate the VAO.
   glGenVertexArrays(1, &id_);

   setAttributes_();
}

/**
   \param usage Expected usage of the VBO (static, dynamic or stream draws).
*/
template<size_t S>
inline rw::gl::VAO<S>::VAO(GLenum usage)
{
   // Generate the VAO.
   glGenVertexArrays(1, &id_);

   vbo_ = rw::gl::Buffer<float, S>(GL_ARRAY_BUFFER, usage);
   setAttributes_();
}

template<size_t S>
inline rw::gl::VAO<S>::~VAO()
{
   glDeleteVertexArrays(1, &id_);
}

template<size_t S>
inline rw::gl::VAO<S>::VAO(VAO&& oldVAO)
{
   id_ = oldVAO.id_;
   oldVAO.id_ = 0;

   vbo_ = std::move(oldVAO.vbo_);
   ebo_ = std::move(oldVAO.ebo_);

   setAttributes_();
}

template<size_t S>
inline rw::gl::VAO<S>& rw::gl::VAO<S>::operator=(VAO&& oldVAO)
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

template<size_t S>
inline void rw::gl::VAO<S>::bind() const
{
   glBindVertexArray(id_);
}

/**
   \param data Data to store in the EBO.
*/
template<size_t S>
inline void rw::gl::VAO<S>::setEBO(const std::array<unsigned int, ebo_size>& data)
{
   ebo_.setData(data);
}

/**
   \param buff Source data buffer. It will be moved.
*/
template<size_t S>
inline void rw::gl::VAO<S>::setEBO(rw::gl::Buffer<unsigned int, ebo_size>&& buff)
{
   ebo_ = std::move(buff);
   glBindVertexArray(id_);
   ebo_.bind();
}

/**
   \param data Data to store in the VBO.
*/
template<size_t S>
inline void rw::gl::VAO<S>::setVBO(const std::array<float, S>& data)
{
   vbo_.setData(data);
}

/**
   \param buff Source data buffer. It will be moved.
*/
template<size_t S>
inline void rw::gl::VAO<S>::setVBO(rw::gl::Buffer<float, S>&& buff)
{
   vbo_ = std::move(buff);
   setAttributes_();
}

template<size_t S>
inline void rw::gl::VAO<S>::setAttributes_()
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
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[0]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[1]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[2]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[3]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[4]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[5]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[6]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[7]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[8]>;
template class RW_API rw::gl::VAO<rw::gl::vao_sizes[9]>;

#endif