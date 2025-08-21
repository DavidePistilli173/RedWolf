//
// Created by cjm036653 on 21/08/2025.
//

#include "gl_vertex_buffer.hpp"

#include "RedWolf/util/logger.hpp"

#include <glad/glad.h>

rw::gfx::api::gl::VertexBuffer::VertexBuffer() {
    glCreateBuffers(1, &id_);
    RW_CORE_TRACE("Vertex buffer created with ID: {}", id_);
}

rw::gfx::api::gl::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id_);
}

rw::gfx::api::gl::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : id_{ other.id_ } {
    other.id_ = 0;
}

rw::gfx::api::gl::VertexBuffer& rw::gfx::api::gl::VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &id_);
        id_       = other.id_;
        other.id_ = 0;
    }

    return *this;
}

void rw::gfx::api::gl::VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

const rw::gfx::BufferLayout& rw::gfx::api::gl::VertexBuffer::layout() const {
    return layout_;
}

void rw::gfx::api::gl::VertexBuffer::set_data(const std::span<float> vertex_data) {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertex_data.size_bytes()), vertex_data.data(), GL_STATIC_DRAW);
}

void rw::gfx::api::gl::VertexBuffer::set_layout(const BufferLayout& layout) {
    layout_ = layout;
}

void rw::gfx::api::gl::VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}