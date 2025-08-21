//
// Created by cjm036653 on 18/08/2025.
//

#ifndef SRC_REDWOLF_GFX_HPP
#define SRC_REDWOLF_GFX_HPP

#include <cstdint>

#if defined(RW_GFX_API_OPENGL)
    #include "api/gl/gl_context.hpp"
    #include "api/gl/gl_index_buffer.hpp"
    #include "api/gl/gl_renderer_api.hpp"
    #include "api/gl/gl_shader.hpp"
    #include "api/gl/gl_vertex_array.hpp"
    #include "api/gl/gl_vertex_buffer.hpp"
#else
    #error "No rendering API selected."
#endif

namespace rw::gfx {
    /**
     * @brief Available rendering APIs.
     */
    enum class GraphicsApi : uint8_t { opengl };

#if defined(RW_GFX_API_OPENGL)
    using GraphicsContext = rw::gfx::api::gl::Context;
    using IndexBuffer     = rw::gfx::api::gl::IndexBuffer;
    using RendererApi     = rw::gfx::api::gl::RendererApi;
    using Shader          = rw::gfx::api::gl::Shader;
    using VertexArray     = rw::gfx::api::gl::VertexArray;
    using VertexBuffer    = rw::gfx::api::gl::VertexBuffer;

    static constexpr auto active_api{ GraphicsApi::opengl }; /**< Currently selected rendering API. */
#else
    #error "No rendering API selected."
#endif
} // namespace rw::gfx

#endif // SRC_REDWOLF_GFX_HPP
