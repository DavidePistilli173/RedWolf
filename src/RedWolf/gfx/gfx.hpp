//
// Created by cjm036653 on 18/08/2025.
//

#ifndef SRC_REDWOLF_GFX_HPP
#define SRC_REDWOLF_GFX_HPP

#include <cstdint>

#if defined(RW_GFX_API_OPENGL)
    #include "api/gl/gl_context.hpp"
    #include "api/gl/gl_shader.hpp"
#else
    #error "No rendering API selected."
#endif

namespace rw::gfx {
#if defined(RW_GFX_API_OPENGL)
    using GraphicsContext = rw::gfx::api::gl::Context;
    using Shader          = rw::gfx::api::gl::Shader;
#else
    #error "No rendering API selected."
#endif
} // namespace rw::gfx

#endif // SRC_REDWOLF_GFX_HPP
