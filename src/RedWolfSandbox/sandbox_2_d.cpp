//
// Created by david on 12/09/2025.
//

#include "sandbox_2_d.hpp"

#include <RedWolf/engine/app.hpp>

static constexpr uint64_t flat_colored_shader_id{ 0 };

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), camera_controller_{ 1280.0F / 720.0F } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface();
}

void Sandbox2D::attach() {
    renderer_interface_->set_clear_color(rw::math::Vec4(1.0F, 1.0F, 0.0F, 0.0F));

    float square_vertices[] = { -0.5F, -0.5F, 0.0F, 0.5F, -0.5F, 0.0F, 0.5F, 0.5F, 0.0F, -0.5F, 0.5F, 0.0F };

    square_va_ = std::make_shared<rw::gfx::VertexArray>();
    auto square_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    square_vb->set_data(square_vertices);
    square_vb->set_layout(rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" } });

    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     square_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    square_ib->set_data(square_indices);

    square_va_->add_vertex_buffer(square_vb);
    square_va_->set_index_buffer(square_ib);

    shader_ = renderer_interface_->load_shader(flat_colored_shader_id, "../src/RedWolfSandbox/assets/shaders/flat_colored.glsl").get();
}

void Sandbox2D::detach() {}

void Sandbox2D::render_imgui() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(square_color_));
    ImGui::End();
}

void Sandbox2D::update(const float delta_time) {
    // Update
    camera_controller_.update(delta_time);

    // Render
    renderer_interface_->clear_screen();

    renderer_interface_->begin_scene(camera_controller_.camera());

    shader_->bind();
    shader_->upload_uniform_f32_4("u_color", square_color_);

    renderer_interface_->draw(flat_colored_shader_id, square_va_.get(), rw::math::Mat4(1.0F));

    renderer_interface_->end_scene();
}

bool Sandbox2D::on_event(const rw::evt::Event& event) {
    return camera_controller_.on_event(event);
}