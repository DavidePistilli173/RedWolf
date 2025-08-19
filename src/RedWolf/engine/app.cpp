//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "../layers/debug_layer.hpp"
#include "../util/logger.hpp"

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
    glGenVertexArrays(1, &vertex_array_);
    glBindVertexArray(vertex_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

    float vertices[3 * 3] = {
        -0.5F, -0.5F, 0.0F, 0.5F, -0.5F, 0.0F, 0.0F, 0.5F, 0.0F,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);

    uint32_t indices[3] = {
        0,
        1,
        2,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader_ = std::make_unique<rw::gfx::api::gl::Shader>(
        R"(
        #version 330 core

        layout(location = 0) in vec3 in_position;

        out vec3 v_position;

        void main() {
            v_position = in_position * 2;
            gl_Position = vec4(in_position * 2, 1.0);
        }
    )",
        R"(
        #version 330 core

        in vec3 v_position;

        layout(location = 0) out vec4 color;

        void main() {
            color = vec4(v_position * 0.5 + 0.5, 1.0);
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
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
