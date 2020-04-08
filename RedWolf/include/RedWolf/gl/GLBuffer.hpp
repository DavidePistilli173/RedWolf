#ifndef REDWOLF_GL_GLBUFFER_HPP
#define REDWOLF_GL_GLBUFFER_HPP

#include "RedWolf/core.hpp"

#include <glad/glad.h>

#include <array>

namespace rw::gl
{
    /** \brief Wrapper for an OpenGL data buffer. */
    template <typename T, size_t N>
    class GLBuffer
    {
    public:
        /** \brief Create an empty buffer with default settings. */
        GLBuffer();

        /** \brief Create an empty buffer with a custom target. */
        explicit GLBuffer(GLenum target);

        /** \brief Create an empty buffer with custom target and usage parameters. */
        GLBuffer(GLenum target, GLenum usage);

        /** \brief Create a custom buffer and fill it with data. */
        GLBuffer(const std::array<T,N>& data, GLenum target, GLenum usage);

        /** \brief Destructor. */
        ~GLBuffer();

        /** \brief This object cannot be copied. */
        GLBuffer(const GLBuffer&) = delete;

        /** \brief Move constructor. */
        GLBuffer(GLBuffer&& buf) noexcept;

        /** \brief This object cannot be copied. */
        GLBuffer& operator=(const GLBuffer&) = delete;

        /** \brief Move-assignment operator. */
        GLBuffer& operator=(GLBuffer&& buf) noexcept;

        /** \brief Bind the buffer to its current target. */
        void bind() const noexcept;

        /** \brief Set the buffer's data. */
        void setData(const std::array<T, N>& data);

        /** \brief Set the buffer's target. */
        void setTarget(GLenum target);

    private:
        GLuint id_{ 0 }; /**< Buffer id. */
        size_t capacity_{ 0 }; /**< Current capacity of the buffer (number of elements). */
        GLenum target_{ GL_ARRAY_BUFFER }; /**< Binding target of the buffer. */
        GLenum usage_{ GL_STATIC_DRAW }; /**< static, dynamic or stream drawing. */
    };
}


template <typename T, size_t N>
inline rw::gl::GLBuffer<T, N>::GLBuffer()
{
    glGenBuffers(1, &id_);
}

/**
    \param target Binding target for the buffer.
*/
template<typename T, size_t N>
inline rw::gl::GLBuffer<T, N>::GLBuffer(GLenum target) :
    target_{ target }
{
    glGenBuffers(1, &id_);
}

/**
    \param target Binding target for the buffer.
    \param usage Expected usage of the buffer (static, dynamic or stream draws).
*/
template<typename T, size_t N>
inline rw::gl::GLBuffer<T, N>::GLBuffer(GLenum target, GLenum usage) :
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
inline rw::gl::GLBuffer<T, N>::GLBuffer(const std::array<T, N>& data, GLenum target, GLenum usage) :
    capacity_{ data.size() },
    target_{ target },
    usage_{ usage }
{
    glGenBuffers(1, &id_);
    glBindBuffer(target, id_);
    glBufferData(target, data.size() * sizeof(T), data.data(), usage);
}

template<typename T, size_t N>
inline rw::gl::GLBuffer<T, N>::~GLBuffer()
{
    glDeleteBuffers(1, &id_);
}

template<typename T, size_t N>
inline rw::gl::GLBuffer<T, N>::GLBuffer(GLBuffer&& buf) noexcept :
    capacity_{ buf.capacity_ },
    target_{ buf.target_ },
    usage_{ buf.usage_ }
{
    id_ = buf.id_;
    buf.id_ = 0;
}

template<typename T, size_t N>
inline rw::gl::GLBuffer<T, N>& rw::gl::GLBuffer<T, N>::operator=(rw::gl::GLBuffer<T, N>&& buf) noexcept
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
inline void rw::gl::GLBuffer<T, N>::bind() const noexcept
{
    glBindBuffer(target_, id_);
}

/**
    \param data Data to store in the buffer.
*/
template<typename T, size_t N>
inline void rw::gl::GLBuffer<T, N>::setData(const std::array<T, N>& data)
{
    glBindBuffer(target_, id_);
    if (capacity_ == 0) glBufferData(target_, data.size() * sizeof(T), data.data(), usage_);
    else glBufferSubData(target_, 0, data.size() * sizeof(T), data.data());
}

/**
    \param target New binding target for the buffer.
*/
template<typename T, size_t N>
inline void rw::gl::GLBuffer<T, N>::setTarget(GLenum target)
{
    target_ = target;
}

#endif