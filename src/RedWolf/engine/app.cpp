//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "RedWolf/layers/debug_layer.hpp"
#include "RedWolf/util/logger.hpp"

#include <memory>
#include <ranges>

rw::engine::App* rw::engine::App::instance_{ nullptr };

rw::engine::App::App(const rw::ui::WindowDescriptor& window_data) {
    // Initialise the logger as first instruction.
    RW_CORE_INFO("Constructing application");

    // Ensure only one instance of the application exists.
    if (nullptr != instance_) {
        RW_CORE_FATAL("Application instance already exists. Only one instance of App is allowed.");
        return;
    }
    instance_ = this;

    // Create the main application window.
    window_ = std::make_unique<rw::ui::Window>(window_data);
    window_->set_event_callback([this](const rw::evt::Event& event) { return on_event(event); });

    renderer_interface_2d_ = window_->renderer_interface_2d();

    debug_layer_ = dynamic_cast<rw::layers::DebugLayer*>(layer_stack_.push_layer<rw::layers::DebugLayer>());
    if (nullptr == debug_layer_) {
        RW_CORE_FATAL("Failed to create debug layer.");
        return;
    }
}

rw::engine::App::~App() {
    instance_ = nullptr;
}

rw::engine::App& rw::engine::App::get() {
    return *instance_;
}

bool rw::engine::App::on_event(const rw::evt::Event& event) {
    bool result{ false };

    switch (event.type()) {
    case rw::evt::EventType::window_resize:
        on_window_resize_(dynamic_cast<const rw::evt::WindowResizedEvent&>(event));
        result = false;
        break;
    case rw::evt::EventType::window_close:
        running_ = false;
        result   = true;
        break;
    default:
        break;
    }

    // Reverse loop to let the overlays get events before normal layers.
    for (std::ranges::reverse_view rv{ layer_stack_ }; auto& layer : rv) {
        if (layer->on_event(event)) {
            result = true;
            break;
        }
    }

    return result;
}

void rw::engine::App::run() {
    auto last_ts{ std::chrono::high_resolution_clock::now() };

    // Main application loop.
    while (running_) {
        auto       current_ts{ std::chrono::high_resolution_clock::now() };
        const auto delta_time{ static_cast<float>((current_ts - last_ts).count()) * nanoseconds_to_seconds };

        // Don't update the layers if the application is minimised.
        if (!minimized_) {
            for (auto& layer : layer_stack_) {
                layer->update(delta_time);
            }
        }

        // Always update the debug UI since it could be in a separate window.
        debug_layer_->begin_frame();
        for (auto& layer : layer_stack_) {
            layer->render_imgui();
        }
        debug_layer_->end_frame();

        window_->update();

        last_ts = current_ts;
    }
}

rw::ui::Window& rw::engine::App::window() {
    return *window_;
}

void rw::engine::App::on_window_resize_(const rw::evt::WindowResizedEvent& event) {
    if (0 == event.width && 0 == event.height) {
        minimized_ = true;
        return;
    }
    minimized_ = false;

    renderer_interface_2d_->set_viewport(0, 0, event.width, event.height);
}