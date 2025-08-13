//
// Created by cjm036653 on 10/08/25.
//

#include "app.hpp"

rw::engine::App::App() {
    window_ = std::make_unique<rw::ui::Window>(rw::ui::WindowDescriptor{
        .logger = logger_, .title = "RedWolf Engine", .width = rw::ui::default_window_width, .height = rw::ui::default_window_height });
}

rw::util::Logger& rw::engine::App::logger() {
    return logger_;
}

void rw::engine::App::run() {
    while (running_) {
        window_->update();
    }
}