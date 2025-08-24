//
// Created by cjm036653 on 10/08/25.
//

#ifndef SANDBOX_APP_HPP
#define SANDBOX_APP_HPP

#include "example_layer.hpp"

#include <RedWolf/engine/app.hpp>

class SandboxApp : public rw::engine::App {
 public:
    static constexpr rw::ui::WindowDescriptor window_settings{
        .title  = "RedWolf Sandbox",
        .width  = rw::ui::default_window_width,
        .height = rw::ui::default_window_height,
    };

    SandboxApp();
    ~SandboxApp() override                   = default;
    SandboxApp(const SandboxApp&)            = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    SandboxApp(SandboxApp&&)                 = default;
    SandboxApp& operator=(SandboxApp&&)      = default;

 private:
    ExampleLayer* example_layer_{ nullptr };
};

#endif // SANDBOX_APP_HPP
