//
// Created by david on 24/08/2025.
//

#include "example_layer.hpp"

#include <RedWolf/engine/app.hpp>
#include <RedWolf/gfx/gfx.hpp>
#include <RedWolf/input/polling.hpp>
#include <RedWolf/util/logger.hpp>

ExampleLayer::ExampleLayer() : Layer("Sandbox Example Layer"), camera_{ rw::gfx::Camera::orthographic(-1.6F, 1.6F, -0.9F, 0.9F) } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface();
    renderer_interface_->set_clear_color(rw::math::Vec4(1.0F, 1.0F, 0.0F, 0.0F));

    vertex_array_ = std::make_unique<rw::gfx::VertexArray>();

    float vertices[] = { -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.5F, -0.5F, 0.0F, 0.0F,
                         1.0F,  0.0F,  1.0F, 0.0F, 0.5F, 0.0F, 0.0F, 0.0F, 1.0F,  1.0F };
    auto  vertex_buffer{ std::make_shared<rw::gfx::VertexBuffer>() };
    vertex_buffer->set_data(vertices);
    vertex_buffer->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { rw::gfx::ShaderDataType::f32_4, "in_color" } });

    uint32_t indices[3] = {
        0,
        1,
        2,
    };
    auto index_buffer{ std::make_shared<rw::gfx::IndexBuffer>() };
    index_buffer->set_data(indices);

    vertex_array_->add_vertex_buffer(vertex_buffer);
    vertex_array_->set_index_buffer(index_buffer);

    float square_vertices[] = { -0.9F, -0.9F, 0.0F, 0.5F, 0.5F, 0.0F, 1.0F, -0.7F, -0.9F, 0.0F, 0.0F,  0.5F,  0.5F,  1.0F,
                                -0.7F, -0.7F, 0.0F, 0.5F, 0.0F, 0.5F, 1.0F, -0.9F, -0.7F, 0.0F, 0.25F, 0.25F, 0.25F, 1.0F };

    square_va_ = std::make_unique<rw::gfx::VertexArray>();
    auto square_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    square_vb->set_data(square_vertices);
    square_vb->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { rw::gfx::ShaderDataType::f32_4, "in_color" } });

    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     square_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    square_ib->set_data(square_indices);

    square_va_->add_vertex_buffer(square_vb);
    square_va_->set_index_buffer(square_ib);

    shader_ = std::make_unique<rw::gfx::api::gl::Shader>(
        R"(
        #version 330 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        uniform mat4 u_view_projection;

        out vec3 v_position;
        out vec4 v_color;

        void main() {
            v_position = in_position;
            v_color = in_color;
            gl_Position = u_view_projection * vec4(in_position, 1.0);
        }
    )",
        R"(
        #version 330 core

        in vec3 v_position;
        in vec4 v_color;

        layout(location = 0) out vec4 color;

        void main() {
            color = v_color;
        }
    )");
}

void ExampleLayer::attach() {}

void ExampleLayer::detach() {}

void ExampleLayer::render_imgui() {}

void ExampleLayer::update(const float delta_time) {
    if (rw::input::is_key_down(rw::input::Key::a)) {
        speed_x = -1.0F;
    } else if (rw::input::is_key_down(rw::input::Key::d)) {
        speed_x = 1.0F;
    } else {
        speed_x = 0.0F;
    }

    if (rw::input::is_key_down(rw::input::Key::s)) {
        speed_y = -1.0F;
    } else if (rw::input::is_key_down(rw::input::Key::w)) {
        speed_y = 1.0F;
    } else {
        speed_y = 0.0F;
    }

    if (rw::input::is_key_down(rw::input::Key::q)) {
        rotation_speed_ = 4.0F;
    } else if (rw::input::is_key_down(rw::input::Key::e)) {
        rotation_speed_ = -4.0F;
    } else {
        rotation_speed_ = 0.0F;
    }

    camera_.roto_translate(rw::math::Vec3(speed_x, speed_y, 0.0F) * delta_time, rotation_speed_);

    renderer_interface_->clear_screen();
    renderer_interface_->begin_scene(camera_);
    renderer_interface_->draw(shader_.get(), vertex_array_.get());
    renderer_interface_->draw(shader_.get(), square_va_.get());
    renderer_interface_->end_scene();
}

bool ExampleLayer::on_event(const rw::evt::Event& event) {
    return false;
}