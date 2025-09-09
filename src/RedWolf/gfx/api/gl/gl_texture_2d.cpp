//
// Created by david on 08/09/2025.
//

#include "gl_texture_2d.hpp"

#include "RedWolf/util/logger.hpp"

#include <glad/glad.h>
#include <stb/stb_image.h>

rw::gfx::api::gl::Texture2D::Texture2D(const std::string_view path) : path_{ path } {
    int channels{ 0 };
    int width{ 0 };
    int height{ 0 };
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* texture_data{ stbi_load(path_.c_str(), &width, &height, &channels, 0) };
    if (nullptr == texture_data) {
        RW_CORE_ERR("Failed to load image: {}", path_);
        return;
    }

    if (0 > width || 0 > height) {
        RW_CORE_ERR("Invalid image dimensions: {}x{} for image: {}", width, height, path_);
        stbi_image_free(texture_data);
        return;
    }
    width_  = width;
    height_ = height;

    // Adjust the format depending on the loaded channels.
    GLenum internal_format{ GL_RGB8 };
    GLenum data_format{ GL_RGB };
    if (4 == channels) {
        internal_format = GL_RGBA8;
        data_format     = GL_RGBA;
    } else if (3 == channels) {
        internal_format = GL_RGB8;
        data_format     = GL_RGB;
    } else if (1 == channels) {
        internal_format = GL_R8;
        data_format     = GL_RED;
    } else {
        RW_CORE_ERR("Unsupported number of channels: {} for image: {}", channels, path_);
        return;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &id_);
    glTextureStorage2D(id_, 1, internal_format, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));

    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(id_, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, texture_data);
    stbi_image_free(texture_data);

    RW_CORE_TRACE("Texture2D created: {} | id = {}", path_, id_);
}

rw::gfx::api::gl::Texture2D::~Texture2D() {
    glDeleteTextures(1, &id_);
}

rw::gfx::api::gl::Texture2D::Texture2D(Texture2D&& other) noexcept :
    id_{ other.id_ }, path_{ std::move(other.path_) }, width_{ other.width_ }, height_{ other.height_ } {
    other.id_ = 0U;
}

rw::gfx::api::gl::Texture2D& rw::gfx::api::gl::Texture2D::operator=(Texture2D&& other) noexcept {
    if (this != &other) {
        id_       = other.id_;
        path_     = std::move(other.path_);
        width_    = other.width_;
        height_   = other.height_;
        other.id_ = 0U;
    }
    return *this;
}

void rw::gfx::api::gl::Texture2D::bind(const uint32_t slot) const {
    glBindTextureUnit(slot, id_);
}

uint32_t rw::gfx::api::gl::Texture2D::height() const {
    return height_;
}

const std::string& rw::gfx::api::gl::Texture2D::path() const {
    return path_;
}

uint32_t rw::gfx::api::gl::Texture2D::width() const {
    return width_;
}