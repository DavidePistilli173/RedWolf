module;

#include "RedWolf/macros.hpp"

#include <glad/glad.h>

module redwolf.gfx.vertex_buffer;

import redwolf.gfx.buffer_layout;
import redwolf.util.logger;

rw::gfx::VertexBuffer::VertexBuffer() {
    glCreateBuffers(1, &id_);
    RW_CORE_TRACE("Vertex buffer {} created.", id_);
}

rw::gfx::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id_);
    RW_CORE_TRACE("Vertex buffer {} deleted.", id_);
}

rw::gfx::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : id_{ other.id_ } {
    other.id_ = 0;
}

rw::gfx::VertexBuffer& rw::gfx::VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &id_);
        id_       = other.id_;
        other.id_ = 0;
    }

    return *this;
}

void rw::gfx::VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

const rw::gfx::BufferLayout& rw::gfx::VertexBuffer::layout() const {
    return layout_;
}

void rw::gfx::VertexBuffer::set_layout(const BufferLayout& layout) {
    layout_ = layout;
}

void rw::gfx::VertexBuffer::set_size(const uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void rw::gfx::VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}