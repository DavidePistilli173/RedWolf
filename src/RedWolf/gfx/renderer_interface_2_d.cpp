//
// Created by david on 12/09/2025.
//

#include "renderer_interface_2_d.hpp"
rw::gfx::RendererInterface2D::RendererInterface2D(std::shared_ptr<Renderer2D> renderer) : renderer_{ std::move(renderer) } {}

void rw::gfx::RendererInterface2D::begin_scene(const Camera& camera) {
    renderer_->begin_scene(camera);
}

void rw::gfx::RendererInterface2D::clear_screen() {
    renderer_->clear_screen();
}

std::future<rw::Handle<rw::gfx::Framebuffer>>
    rw::gfx::RendererInterface2D::create_framebuffer(const uint64_t id, const FramebufferDescriptor& descriptor) {
    std::promise<Handle<Framebuffer>> promise;
    promise.set_value(renderer_->create_framebuffer(id, descriptor));
    return promise.get_future();
}

void rw::gfx::RendererInterface2D::draw_quad(rw::Handle<Shader> shader, const Quad& quad) {
    renderer_->draw_quad(shader, quad);
}

void rw::gfx::RendererInterface2D::end_scene() {
    renderer_->end_scene();
}

std::future<rw::Handle<rw::gfx::Shader>> rw::gfx::RendererInterface2D::get_shader(const uint64_t id) {
    std::promise<Handle<Shader>> promise;
    promise.set_value(renderer_->get_shader(id));
    return promise.get_future();
}

std::future<rw::Handle<rw::gfx::Texture2D>> rw::gfx::RendererInterface2D::get_texture(const uint64_t id) {
    std::promise<Handle<Texture2D>> promise;
    promise.set_value(renderer_->get_texture(id));
    return promise.get_future();
}

std::future<rw::Handle<rw::gfx::Texture2D>> rw::gfx::RendererInterface2D::load_texture(const uint64_t id, const std::string& file_path) {
    std::promise<Handle<Texture2D>> promise;
    promise.set_value(renderer_->load_texture(id, file_path));
    return promise.get_future();
}

void rw::gfx::RendererInterface2D::reset_stats() {
    renderer_->reset_stats();
}

void rw::gfx::RendererInterface2D::set_viewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
    renderer_->set_viewport(x, y, width, height);
}

std::future<rw::gfx::Renderer2DStats> rw::gfx::RendererInterface2D::stats() const {
    std::promise<Renderer2DStats> promise;
    promise.set_value(renderer_->stats());
    return promise.get_future();
}