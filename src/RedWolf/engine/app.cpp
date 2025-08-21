//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "../layers/debug_layer.hpp"
#include "../util/logger.hpp"
#include "RedWolf/gfx/shader_data.hpp"

#include <glad/glad.h>
#include <memory>
#include <ranges>

static constexpr GLenum shader_data_type_to_gl_type(const rw::gfx::ShaderDataType type) {
    switch (type) {
    case rw::gfx::ShaderDataType::none:
        return GL_NONE;
    case rw::gfx::ShaderDataType::f32:
    case rw::gfx::ShaderDataType::f32_2:
    case rw::gfx::ShaderDataType::f32_3:
    case rw::gfx::ShaderDataType::f32_4:
    case rw::gfx::ShaderDataType::mat_f32_3:
    case rw::gfx::ShaderDataType::mat_f32_4:
        return GL_FLOAT;
    case rw::gfx::ShaderDataType::i32:
    case rw::gfx::ShaderDataType::i32_2:
    case rw::gfx::ShaderDataType::i32_3:
    case rw::gfx::ShaderDataType::i32_4:
        return GL_INT;
    case rw::gfx::ShaderDataType::boolean:
        return GL_BOOL;
    default:
        return GL_NONE; // Invalid type
    }
}

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
    glGenVertexArrays(1, &vertex_array_);
    glBindVertexArray(vertex_array_);

    float vertices[] = { -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.5F, -0.5F, 0.0F, 0.0F,
                         1.0F,  0.0F,  1.0F, 0.0F, 0.5F, 0.0F, 0.0F, 0.0F, 1.0F,  1.0F };
    vertex_buffer_   = std::make_unique<rw::gfx::VertexBuffer>();
    vertex_buffer_->set_data(vertices);
    vertex_buffer_->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { rw::gfx::ShaderDataType::f32_4, "in_color" } });

    uint32_t index{ 0U };
    for (const auto& element : vertex_buffer_->layout()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            static_cast<int32_t>(element.component_count()),
            shader_data_type_to_gl_type(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            static_cast<int32_t>(vertex_buffer_->layout().stride()),
            reinterpret_cast<const void*>(element.offset));
        ++index;
    }

    uint32_t indices[3] = {
        0,
        1,
        2,
    };
    index_buffer_ = std::make_unique<rw::gfx::IndexBuffer>();
    index_buffer_->set_data(indices);

    shader_ = std::make_unique<rw::gfx::api::gl::Shader>(
        R"(
        #version 330 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        out vec3 v_position;
        out vec4 v_color;

        void main() {
            v_position = in_position * 2;
            v_color = in_color;
            gl_Position = vec4(in_position * 2, 1.0);
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
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_->bind();
        glBindVertexArray(vertex_array_);
        glDrawElements(GL_TRIANGLES, index_buffer_->count(), GL_UNSIGNED_INT, nullptr);

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
