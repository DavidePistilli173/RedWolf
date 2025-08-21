//
// Created by cjm036653 on 21/08/2025.
//

#include "gl_index_buffer.hpp"

#include "RedWolf/util/logger.hpp"

#include <glad/glad.h>

rw::gfx::api::gl::IndexBuffer::IndexBuffer() {
    glCreateBuffers(1, &id_);
    RW_CORE_TRACE("Index buffer created with ID: {}", id_);
}

rw::gfx::api::gl::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id_);
}

rw::gfx::api::gl::IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : id_{ other.id_ } {
    other.id_ = 0;
}

rw::gfx::api::gl::IndexBuffer& rw::gfx::api::gl::IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &id_);
        id_       = other.id_;
        other.id_ = 0;
    }

    return *this;
}

void rw::gfx::api::gl::IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

int32_t rw::gfx::api::gl::IndexBuffer::count() const {
    return count_;
}

void rw::gfx::api::gl::IndexBuffer::set_data(const std::span<uint32_t> index_data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(index_data.size_bytes()), index_data.data(), GL_STATIC_DRAW);
    count_ = static_cast<int32_t>(index_data.size());
}

void rw::gfx::api::gl::IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}