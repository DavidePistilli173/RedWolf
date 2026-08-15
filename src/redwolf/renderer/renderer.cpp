#include "renderer.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/renderer/backends/renderer_backends.hpp"
#include "redwolf/renderer/backends/vulkan/vulkan_renderer.hpp"

namespace {
    rw::Renderer* g_renderer{ nullptr }; // Renderer front-end instance.
}

bool rw::Renderer::draw_frame(const RenderPacket& packet) {
    if (!RendererBackend::begin_frame(packet.delta_time)) {
        warn("Cannot begin new frame. Skipping frame.");
        return true; // Not an error.
    }

    if (!RendererBackend::end_frame(packet.delta_time)) {
        error("Cannot end frame.");
        return false;
    }

    return true;
}

bool rw::Renderer::init() {
    if (nullptr != g_renderer) {
        warn("Renderer front-end already initialised.");
        return true;
    }

    g_renderer = new Renderer();

    if (!RendererBackend::init()) {
        error("Failed to initialise renderer backend.");
        return false;
    }

    return true;
}

void rw::Renderer::shutdown() {
    if (nullptr == g_renderer) {
        warn("Renderer front-end already shut down.");
        return;
    }

    RendererBackend::shutdown();
    delete g_renderer;
    g_renderer = nullptr;
}