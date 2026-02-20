module;

#include <glad/glad.h>

module redwolf.gfx.index_buffer;

import redwolf.util.logger;

rw::gfx::IndexBuffer::IndexBuffer() {
    glCreateBuffers(1, &id_);
    rw::trace("Index buffer {} created.", id_);
}

rw::gfx::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id_);
    rw::trace("Index buffer {} deleted.", id_);
}

rw::gfx::IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : id_{ other.id_ } {
    other.id_ = 0;
}

rw::gfx::IndexBuffer& rw::gfx::IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &id_);
        id_       = other.id_;
        other.id_ = 0;
    }

    return *this;
}

void rw::gfx::IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

int32_t rw::gfx::IndexBuffer::count() const {
    return count_;
}

void rw::gfx::IndexBuffer::set_data(const std::span<uint32_t> index_data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(index_data.size_bytes()), index_data.data(), GL_STATIC_DRAW);
    count_ = static_cast<int32_t>(index_data.size());
}

void rw::gfx::IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}