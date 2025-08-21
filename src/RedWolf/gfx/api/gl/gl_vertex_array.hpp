//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_VERTEX_ARRAY_HPP
#define SRC_REDWOLF_GL_VERTEX_ARRAY_HPP

#include "gl_index_buffer.hpp"
#include "gl_vertex_buffer.hpp"

#include <memory>
#include <vector>

namespace rw::gfx::api::gl {
    /**
     * @brief OpenGL vertex array object.
     */
    class VertexArray {
     public:
        /**
         * @brief Constructor.
         */
        explicit VertexArray();

        /**
         * @brief Destructor.
         */
        ~VertexArray();

        /**
         * @brief Copy constructor.
         */
        VertexArray(const VertexArray&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        VertexArray& operator=(const VertexArray&) = delete;

        /**
         * @brief Move constructor.
         */
        VertexArray(VertexArray&& other) noexcept;

        /**
         * @brief Move-assignment operator.
         */
        VertexArray& operator=(VertexArray&& other) noexcept;

        /**
         * @brief Add a vertex buffer to the vertex array.
         * @param vertex_buffer Vertex buffer to add.
         */
        void add_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer);

        /**
         * @brief Set the index buffer of the vertex array.
         * @param index_buffer Index buffer to set.
         */
        void set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer);

        /**
         * @brief Bind the buffer.
         */
        void bind() const;

        /**
         * @brief Get the index buffer bound to the vertex array.
         * @return Index buffer bound to the vertex array.
         */
        [[nodiscard]] const std::shared_ptr<IndexBuffer>& index_buffer() const;

        /**
         * @brief Get the vertex buffers bound to the vertex array.
         * @return Vertex buffers bound to the vertex array.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const;

        /**
         * @brief Unbind the buffer.
         */
        void unbind() const;

     private:
        uint32_t                                   id_{ 0U };       /**< Vertex array ID. */
        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_; /**< Vertex buffers. */
        std::shared_ptr<IndexBuffer>               index_buffer_;   /**< Index buffer. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_VERTEX_ARRAY_HPP
