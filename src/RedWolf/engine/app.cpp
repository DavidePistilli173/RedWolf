//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "../util/logger.hpp"

#include <memory>
#include <ranges>

rw::engine::App::App() {
    RW_CORE_INFO("Constructing application");

    if (nullptr != instance_) {
        RW_CORE_FATAL("Application instance already exists. Only one instance of App is allowed.");
        return;
    }
    instance_ = this;

    window_ = std::make_unique<rw::ui::Window>(rw::ui::WindowDescriptor{
        .title = "RedWolf Engine", .width = rw::ui::default_window_width, .height = rw::ui::default_window_height });
    window_->set_event_callback([this](const rw::evt::Event& event) { return on_event(event); });
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
        for (auto& layer : layer_stack_) {
            layer->update();
        }

        window_->update();
    }
}

rw::ui::Window& rw::engine::App::window() {
    return *window_;
}
