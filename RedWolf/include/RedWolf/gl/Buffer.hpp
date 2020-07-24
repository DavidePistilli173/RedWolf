#ifndef REDWOLF_GL_BUFFER_HPP
#define REDWOLF_GL_BUFFER_HPP

#include "RedWolf/core.hpp"

#include <array>
#include <glad/glad.h>

namespace rw::gl
{
   /** \brief Wrapper for an OpenGL data buffer. */
   template<typename T, size_t N>
   class Buffer
   {
   public:
      /** \brief Create an empty buffer with default settings. */
      Buffer();

      /** \brief Create an empty buffer with a custom target. */
      explicit Buffer(GLenum target);

      /** \brief Create an empty buffer with custom target and usage parameters. */
      Buffer(GLenum target, GLenum usage);

      /** \brief Create a custom buffer and fill it with data. */
      Buffer(const std::array<T, N>& data, GLenum target, GLenum usage);

      /** \brief Destructor. */
      ~Buffer();

      /** \brief This object cannot be copied. */
      Buffer(const Buffer&) = delete;

      /** \brief Move constructor. */
      Buffer(Buffer&& buf) noexcept;

      /** \brief This object cannot be copied. */
      Buffer& operator=(const Buffer&) = delete;

      /** \brief Move-assignment operator. */
      Buffer& operator=(Buffer&& buf) noexcept;

      /** \brief Bind the buffer to its current target. */
      void bind() const noexcept;

      /** \brief Set the buffer's data. */
      void setData(const std::array<T, N>& data);

      /** \brief Set the buffer's target. */
      void setTarget(GLenum target);

   private:
      GLuint id_{ 0 };                   /**< Buffer id. */
      size_t capacity_{ 0 };             /**< Current capacity of the buffer (number of elements). */
      GLenum target_{ GL_ARRAY_BUFFER }; /**< Binding target of the buffer. */
      GLenum usage_{ GL_STATIC_DRAW };   /**< static, dynamic or stream drawing. */
   };

   /** \brief Supported buffer sizes. */
   constexpr std::array buffer_sizes{
      1, 3, 6, 20, 60, 64
   };
} // namespace rw::gl

template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::Buffer()
{
   glGenBuffers(1, &id_);
}

/**
    \param target Binding target for the buffer.
*/
template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::Buffer(GLenum target) :
   target_{ target }
{
   glGenBuffers(1, &id_);
}

/**
    \param target Binding target for the buffer.
    \param usage Expected usage of the buffer (static, dynamic or stream draws).
*/
template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::Buffer(GLenum target, GLenum usage) :
   target_{ target },
   usage_{ usage }
{
   glGenBuffers(1, &id_);
}

/**
    \param data Data to store in the buffer.
    \param target Binding target for the buffer.
    \param usage Expected usage of the buffer (static, dynamic or stream draws).
*/
template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::Buffer(const std::array<T, N>& data, GLenum target, GLenum usage) :
   capacity_{ data.size() },
   target_{ target },
   usage_{ usage }
{
   glGenBuffers(1, &id_);
   glBindBuffer(target, id_);
   glBufferData(target, data.size() * sizeof(T), data.data(), usage);
}

template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::~Buffer()
{
   glDeleteBuffers(1, &id_);
}

template<typename T, size_t N>
inline rw::gl::Buffer<T, N>::Buffer(Buffer&& buf) noexcept :
   capacity_{ buf.capacity_ },
   target_{ buf.target_ },
   usage_{ buf.usage_ }
{
   id_ = buf.id_;
   buf.id_ = 0;
}

template<typename T, size_t N>
inline rw::gl::Buffer<T, N>& rw::gl::Buffer<T, N>::operator=(rw::gl::Buffer<T, N>&& buf) noexcept
{
   if (this != &buf)
   {
      glDeleteBuffers(1, &id_);
      id_ = buf.id_;
      buf.id_ = 0;

      capacity_ = buf.capacity_;
      target_ = buf.target_;
      usage_ = buf.usage_;
   }
   return *this;
}

template<typename T, size_t N>
inline void rw::gl::Buffer<T, N>::bind() const noexcept
{
   glBindBuffer(target_, id_);
}

/**
    \param data Data to store in the buffer.
*/
template<typename T, size_t N>
inline void rw::gl::Buffer<T, N>::setData(const std::array<T, N>& data)
{
   glBindBuffer(target_, id_);
   if (capacity_ == 0)
      glBufferData(target_, data.size() * sizeof(T), data.data(), usage_);
   else
      glBufferSubData(target_, 0, data.size() * sizeof(T), data.data());
}

/**
    \param target New binding target for the buffer.
*/
template<typename T, size_t N>
inline void rw::gl::Buffer<T, N>::setTarget(GLenum target)
{
   target_ = target;
}

// Template instantiations.
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[0]>;
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[1]>;
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[2]>;
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[3]>;
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[4]>;
template class RW_API rw::gl::Buffer<unsigned int, rw::gl::buffer_sizes[5]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[0]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[1]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[2]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[3]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[4]>;
template class RW_API rw::gl::Buffer<float, rw::gl::buffer_sizes[5]>;

#endif