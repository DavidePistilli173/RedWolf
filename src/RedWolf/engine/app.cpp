//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "../layers/debug_layer.hpp"
#include "../util/logger.hpp"
#include "RedWolf/gfx/renderer.hpp"
#include "RedWolf/gfx/renderer_interface.hpp"
#include "RedWolf/gfx/shader_data.hpp"

#include <glad/glad.h>
#include <memory>
#include <ranges>

rw::engine::App::App(const rw::ui::WindowDescriptor& window_data) {
    // Initialise the logger as first instruction.
    RW_CORE_INFO("Constructing application");

    if (nullptr != instance_) {
        RW_CORE_FATAL("Application instance already exists. Only one instance of App is allowed.");
        return;
    }
    instance_ = this;

    window_ = std::make_unique<rw::ui::Window>(window_data);
    window_->set_event_callback([this](const rw::evt::Event& event) { return on_event(event); });

    debug_layer_ = dynamic_cast<rw::layers::DebugLayer*>(layer_stack_.push_layer<rw::layers::DebugLayer>());
    if (nullptr == debug_layer_) {
        RW_CORE_FATAL("Failed to create debug layer.");
        return;
    }

    // TODO: Will be removed.
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

        out vec3 v_position;
        out vec4 v_color;

        void main() {
            v_position = in_position;
            v_color = in_color;
            gl_Position = vec4(in_position, 1.0);
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

rw::engine::App::~App() {
    instance_ = nullptr;
}

rw::engine::App* rw::engine::App::instance_{ nullptr };

rw::engine::App& rw::engine::App::get() {
    return *instance_;
}

bool rw::engine::App::on_event(const rw::evt::Event& event) {
    bool result{ false };

    switch (event.type()) {
    case rw::evt::EventType::window_close:
        running_ = false;
        result   = true;
        break;
    default:
        break;
    }

    // Reverse loop to let the overlays get events before normal layers.
    std::ranges::reverse_view rv{ layer_stack_ };
    for (auto& layer : rv) {
        if (layer->on_event(event)) {
            result = true;
            break;
        }
    }

    return result;
}

void rw::engine::App::run() {
    while (running_) {
        {
            auto renderer_interface{window_->begin_scene()};
            renderer_interface.set_clear_color(rw::math::Vec4(1.0F, 1.0F, 0.0F, 0.0F));
            renderer_interface.clear_screen();

            shader_->bind();
            vertex_array_->bind();
            renderer_interface.draw_indexed(vertex_array_.get());

            square_va_->bind();
            renderer_interface.draw_indexed(square_va_.get());
        }

        for (auto& layer : layer_stack_) {
            layer->update();
        }

        debug_layer_->begin_frame();
        for (auto& layer : layer_stack_) {
            layer->render_imgui();
        }
        debug_layer_->end_frame();

        window_->update();
    }
}

rw::ui::Window& rw::engine::App::window() {
    return *window_;
}
