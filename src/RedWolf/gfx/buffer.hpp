//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_BUFFER_HPP
#define SRC_REDWOLF_BUFFER_HPP

namespace rw::gfx {
    /**
     * @brief Generic vertex buffer class.
     */
    class VertexBuffer {
     public:
        /**
         * @brief Constructor.
         */
        VertexBuffer() = default;

        /**
         * @brief Destructor.
         */
        virtual ~VertexBuffer() = default;

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
        VertexBuffer(VertexBuffer&& other) = default;

        /**
         * @brief Move-assignment operator.
         */
        VertexBuffer& operator=(VertexBuffer&& other) = default;

        /**
         * @brief Bind the buffer.
         */
        virtual void bind() = 0;

        /**
         * @brief Unbind the buffer.
         */
        virtual void unbind() = 0;
    };

    /**
     * @brief Generic index buffer class.
     */
    class IndexBuffer {
     public:
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_BUFFER_HPP
