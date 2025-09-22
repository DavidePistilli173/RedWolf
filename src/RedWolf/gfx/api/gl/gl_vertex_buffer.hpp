//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_VERTEX_BUFFER_HPP
#define SRC_REDWOLF_GL_VERTEX_BUFFER_HPP

#include "RedWolf/gfx/buffer_layout.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <span>

namespace rw::gfx::api::gl {
    /**
     * @brief OpenGL vertex buffer.
     */
    class VertexBuffer {
     public:
        /**
         * @brief Constructor.
         */
        explicit VertexBuffer();

        /**
         * @brief Destructor.
         */
        ~VertexBuffer();

        /**
         * @brief Copy constructor.
         */
        VertexBuffer(const VertexBuffer&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        /**
         * @brief Move constructor.
         */
        VertexBuffer(VertexBuffer&& other) noexcept;

        /**
         * @brief Move-assignment operator.
         */
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        /**
         * @brief Bind the buffer.
         */
        void bind() const;

        /**
         * @brief Get the currently set data layout of the buffer.
         * @return Data layout of the buffer.
         */
        [[nodiscard]] const BufferLayout& layout() const;

        /**
         * @brief Upload data to the buffer.
         * @tparam T Type of the vertex data.
         * @param vertex_data Vertex data to upload.
         */
        template<typename T>
        void set_data(const std::span<const T> vertex_data) {
            glBindBuffer(GL_ARRAY_BUFFER, id_);
            glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertex_data.size_bytes()), vertex_data.data(), GL_DYNAMIC_DRAW);
        }

        /**
         * @brief Set the data layout of the buffer.
         * @param layout Data layout of the buffer.
         */
        void set_layout(const BufferLayout& layout);

        /**
         * @brief Set the size of the buffer.
         * @param size Size to set. [B]
         */
        void set_size(const uint32_t size);

        /**
         * @brief Unbind the buffer.
         */
        void unbind() const;

     private:
        uint32_t     id_{ 0U }; /**< Buffer ID. */
        BufferLayout layout_;   /**< Data layout of the buffer. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_VERTEX_BUFFER_HPP
