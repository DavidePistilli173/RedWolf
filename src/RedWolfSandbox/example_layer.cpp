//
// Created by david on 24/08/2025.
//

#include "example_layer.hpp"

#include <RedWolf/engine/app.hpp>
#include <RedWolf/gfx/gfx.hpp>
#include <RedWolf/input/polling.hpp>
#include <RedWolf/util/logger.hpp>

static constexpr int32_t  texture_slot{ 0 };
static constexpr uint64_t colored_shader_id{ 0 };
static constexpr uint64_t texture_shader_id{ 1 };

ExampleLayer::ExampleLayer() : Layer("Sandbox Example Layer"), camera_controller_{ 1280.0F / 720.0F } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface();
    renderer_interface_->set_clear_color(rw::math::Vec4(1.0F, 1.0F, 0.0F, 0.0F));

    float square_vertices[] = { -0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,
                                0.5F,  0.5F,  0.0F, 1.0F, 1.0F, -0.5F, 0.5F,  0.0F, 0.0F, 1.0F };

    square_va_ = std::make_shared<rw::gfx::VertexArray>();
    auto square_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    square_vb->set_data(square_vertices);
    square_vb->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { rw::gfx::ShaderDataType::f32_2, "in_tex_coord" } });

    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     square_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    square_ib->set_data(square_indices);

    square_va_->add_vertex_buffer(square_vb);
    square_va_->set_index_buffer(square_ib);

    shader_             = renderer_interface_->load_shader(colored_shader_id, "../src/RedWolfSandbox/assets/shaders/colored.glsl").get();
    auto texture_shader = renderer_interface_->load_shader(texture_shader_id, "../src/RedWolfSandbox/assets/shaders/texture.glsl").get();

    texture_             = std::make_shared<rw::gfx::api::gl::Texture2D>("../src/RedWolfSandbox/assets/textures/checkerboard.png");
    transparent_texture_ = std::make_shared<rw::gfx::api::gl::Texture2D>("../src/RedWolfSandbox/assets/textures/ChernoLogo.png");

    texture_shader->bind();
    texture_shader->upload_uniform_i32("u_texture", texture_slot);
}

void ExampleLayer::attach() {}

void ExampleLayer::detach() {}

void ExampleLayer::render_imgui() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(square_color_));
    ImGui::End();
}

void ExampleLayer::update(const float delta_time) {
    camera_controller_.update(delta_time);

    if (rw::input::is_key_down(rw::input::Key::j)) {
        square_pos_.x -= 1.0F * delta_time;
    } else if (rw::input::is_key_down(rw::input::Key::l)) {
        square_pos_.x += 1.0F * delta_time;
    }

    if (rw::input::is_key_down(rw::input::Key::k)) {
        square_pos_.y -= 1.0F * delta_time;
    } else if (rw::input::is_key_down(rw::input::Key::i)) {
        square_pos_.y += 1.0F * delta_time;
    }

    renderer_interface_->clear_screen();
    renderer_interface_->begin_scene(camera_controller_.camera());

    shader_->bind();
    shader_->upload_uniform_f32_4("u_color", square_color_);

    texture_->bind(texture_slot);
    for (int y{ 0 }; y < 20; ++y) {
        for (int x{ 0 }; x < 20; ++x) {
            const rw::math::Mat4 transform{ rw::math::translate(
                rw::math::Mat4(1.0F), rw::math::Vec3{ static_cast<float>(x) * 0.5F, static_cast<float>(y) * 0.5F, 0.0F }) };
            renderer_interface_->draw(colored_shader_id, square_va_.get(), transform);
        }
    }

    // renderer_interface_->draw(shader_.get(), vertex_array_.get(), rw::math::Mat4(1.0F));
    static const rw::math::Mat4 scale{ rw::math::scale(rw::math::Mat4(1.0F), rw::math::Vec3{ 5.0F, 5.0F, 1.0F }) };
    renderer_interface_->draw(
        texture_shader_id,
        square_va_.get(),
        rw::math::scale(rw::math::translate(rw::math::Mat4(1.0F), square_pos_), rw::math::Vec3{ 1.2F, 1.2F, 1.0F }));

    transparent_texture_->bind(texture_slot);
    renderer_interface_->draw(
        texture_shader_id,
        square_va_.get(),
        rw::math::scale(rw::math::translate(rw::math::Mat4(1.0F), square_pos_), rw::math::Vec3{ 1.2F, 1.2F, 1.0F }));
    renderer_interface_->end_scene();
}

bool ExampleLayer::on_event(const rw::evt::Event& event) {
    return camera_controller_.on_event(event);
}