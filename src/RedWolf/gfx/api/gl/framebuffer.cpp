module;

#include "RedWolf/macros.hpp"

#include <glad/glad.h>

module redwolf.gfx.framebuffer;

import redwolf.gfx.framebuffer_descriptor;
import redwolf.util.logger;

rw::gfx::Framebuffer::Framebuffer(const rw::gfx::FramebufferDescriptor& descriptor) : descriptor_{ descriptor } {
    if (recreate()) {
        RW_CORE_TRACE("Framebuffer buffer {} created.", id_);
    }
}
rw::gfx::Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &id_);
    glDeleteTextures(1, &color_attachment_);
    glDeleteTextures(1, &depth_attachment_);
    RW_CORE_TRACE("Framebuffer {} deleted.", id_);
}

rw::gfx::Framebuffer::Framebuffer(Framebuffer&& other) :
    id_{ other.id_ }, color_attachment_{ other.color_attachment_ }, depth_attachment_{ other.depth_attachment_ } {
    other.id_               = 0U;
    other.color_attachment_ = 0U;
    other.depth_attachment_ = 0U;
}

rw::gfx::Framebuffer& rw::gfx::Framebuffer::operator=(Framebuffer&& other) {
    if (this != &other) {
        id_       = other.id_;
        other.id_ = 0U;

        color_attachment_       = other.color_attachment_;
        other.color_attachment_ = 0U;

        depth_attachment_       = other.depth_attachment_;
        other.depth_attachment_ = 0U;
    }
    return *this;
}

void rw::gfx::Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

uint32_t rw::gfx::Framebuffer::id() const {
    return id_;
}

uint32_t rw::gfx::Framebuffer::color_attachment_id() const {
    return color_attachment_;
}

const rw::gfx::FramebufferDescriptor& rw::gfx::Framebuffer::descriptor() const {
    return descriptor_;
}

bool rw::gfx::Framebuffer::recreate() {
    glCreateFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);

    // Create the colour attachment.
    glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment_);
    glBindTexture(GL_TEXTURE_2D, color_attachment_);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        static_cast<int>(descriptor_.width),
        static_cast<int>(descriptor_.height),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

    // Create the depth attachment.
    glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment_);
    glBindTexture(GL_TEXTURE_2D, depth_attachment_);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, static_cast<int>(descriptor_.width), static_cast<int>(descriptor_.height));

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_, 0);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        RW_CORE_ERR("Framebuffer {} is incomplete!", id_);
        return false;
    }

    // Unbind the framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void rw::gfx::Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
