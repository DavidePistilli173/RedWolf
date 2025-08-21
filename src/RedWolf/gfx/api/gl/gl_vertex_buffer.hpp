//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_VERTEX_BUFFER_HPP
#define SRC_REDWOLF_GL_VERTEX_BUFFER_HPP

#include "RedWolf/gfx/buffer_layout.hpp"

#include <cstdint>
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
         * @param vertex_data Vertex data to upload.
         */
        void set_data(const std::span<float> vertex_data);

        /**
         * @brief Set the data layout of the buffer.
         * @param layout Data layout of the buffer.
         */
        void set_layout(const BufferLayout& layout);

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
