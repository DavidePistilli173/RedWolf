//
// Created by cjm036653 on 21/08/2025.
//

#include "gl_vertex_array.hpp"

#include "RedWolf/util/logger.hpp"

#include <glad/glad.h>

namespace {
    constexpr GLenum shader_data_type_to_gl_type(const rw::gfx::ShaderDataType type) {
        switch (type) {
        case rw::gfx::ShaderDataType::none:
            return GL_NONE;
        case rw::gfx::ShaderDataType::f32:
        case rw::gfx::ShaderDataType::f32_2:
        case rw::gfx::ShaderDataType::f32_3:
        case rw::gfx::ShaderDataType::f32_4:
        case rw::gfx::ShaderDataType::mat_f32_3:
        case rw::gfx::ShaderDataType::mat_f32_4:
            return GL_FLOAT;
        case rw::gfx::ShaderDataType::i32:
        case rw::gfx::ShaderDataType::i32_2:
        case rw::gfx::ShaderDataType::i32_3:
        case rw::gfx::ShaderDataType::i32_4:
            return GL_INT;
        case rw::gfx::ShaderDataType::boolean:
            return GL_BOOL;
        default:
            return GL_NONE; // Invalid type
        }
    }
} // namespace

rw::gfx::api::gl::VertexArray::VertexArray() {
    glCreateVertexArrays(1, &id_);
    RW_CORE_TRACE("Created vertex array with ID: {}.", id_);
}

rw::gfx::api::gl::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id_);
}

rw::gfx::api::gl::VertexArray::VertexArray(VertexArray&& other) noexcept :
    id_{ other.id_ }, vertex_buffers_(other.vertex_buffers_), index_buffer_(other.index_buffer_) {
    other.id_ = 0;
}

rw::gfx::api::gl::VertexArray& rw::gfx::api::gl::VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        id_       = other.id_;
        other.id_ = 0;

        vertex_buffers_ = other.vertex_buffers_;
        index_buffer_   = other.index_buffer_;
    }
    return *this;
}

void rw::gfx::api::gl::VertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer) {
    glBindVertexArray(id_);
    vertex_buffer->bind();

    if (vertex_buffer->layout().elements().empty()) {
        RW_CORE_WARN("Adding vertex buffer to vertex array without setting a layout.");
    }

    uint32_t index{ 0U };
    for (const auto& element : vertex_buffer->layout()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            static_cast<int32_t>(element.component_count()),
            shader_data_type_to_gl_type(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            static_cast<int32_t>(vertex_buffer->layout().stride()),
            reinterpret_cast<const void*>(element.offset));
        ++index;
    }
    glBindVertexArray(0);

    vertex_buffers_.emplace_back(std::move(vertex_buffer));
}

void rw::gfx::api::gl::VertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer) {
    glBindVertexArray(id_);
    index_buffer->bind();
    index_buffer_ = std::move(index_buffer);
    glBindVertexArray(0);
}

void rw::gfx::api::gl::VertexArray::bind() const {
    glBindVertexArray(id_);
}

const std::shared_ptr<rw::gfx::api::gl::IndexBuffer>& rw::gfx::api::gl::VertexArray::index_buffer() const {
    return index_buffer_;
}

const std::vector<std::shared_ptr<rw::gfx::api::gl::VertexBuffer>>& rw::gfx::api::gl::VertexArray::vertex_buffers() const {
    return vertex_buffers_;
}

void rw::gfx::api::gl::VertexArray::unbind() const {
    glBindVertexArray(0);
}