//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_INDEX_BUFFER_HPP
#define SRC_REDWOLF_GL_INDEX_BUFFER_HPP

#include <cstdint>
#include <span>

namespace rw::gfx::api::gl {
    /**
     * @brief OpenGL index buffer.
     */
    class IndexBuffer {
     public:
        /**
         * @brief Constructor.
         */
        explicit IndexBuffer();

        /**
         * @brief Destructor.
         */
        ~IndexBuffer();

        /**
         * @brief Copy constructor.
         */
        IndexBuffer(const IndexBuffer&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        /**
         * @brief Move constructor.
         */
        IndexBuffer(IndexBuffer&& other) noexcept;

        /**
         * @brief Move-assignment operator.
         */
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        /**
         * @brief Bind the buffer.
         */
        void bind() const;

        /**
         * @brief Get the number of elements in the buffer.
         * @return Number of elements in the buffer.
         */
        [[nodiscard]] int32_t count() const;

        /**
         * @brief Upload data to the buffer.
         * @param index_data Index data to upload.
         */
        void set_data(const std::span<uint32_t> index_data);

        /**
         * @brief Unbind the buffer.
         */
        void unbind() const;

     private:
        uint32_t id_{ 0U };    /**< Buffer ID. */
        int32_t  count_{ 0U }; /**< Number of elements in the buffer. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_INDEX_BUFFER_HPP
