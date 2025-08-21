//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_VERTEX_BUFFER_HPP
#define SRC_REDWOLF_GL_VERTEX_BUFFER_HPP

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
         * @brief Upload data to the buffer.
         * @param vertex_data Vertex data to upload.
         */
        void set_data(const std::span<float> vertex_data);

        /**
         * @brief Unbind the buffer.
         */
        void unbind() const;

     private:
        uint32_t id_{ 0U }; /**< Buffer ID. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_VERTEX_BUFFER_HPP
