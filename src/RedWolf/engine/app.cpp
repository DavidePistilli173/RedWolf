//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

#include "../util/logger.hpp"

#include <memory>

rw::engine::App::App() {
    window_ = std::make_unique<rw::ui::Window>(rw::ui::WindowDescriptor{
        .logger = logger_, .title = "RedWolf Engine", .width = rw::ui::default_window_width, .height = rw::ui::default_window_height });
    window_->set_event_callback([this](const rw::evt::Event& event) { on_event(event); });
}

rw::util::Logger& rw::engine::App::logger() {
    return logger_;
}

void rw::engine::App::on_event(const rw::evt::Event& event) {
    switch (event.type()) {
    case rw::evt::EventType::window_close:
        running_ = false;
        break;
    default:
        break;
    }
}

void rw::engine::App::run() {
    while (running_) {
        window_->update();
    }
}