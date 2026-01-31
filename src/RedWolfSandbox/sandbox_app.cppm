module;

#include <Redwolf/macros.hpp>
#include <memory>

export module sandbox.app;

import redwolf;
import sandbox.sandbox2d;

export class SandboxApp : public rw::engine::App {
 public:
    static constexpr rw::ui::WindowDescriptor window_settings{
        .title  = "RedWolf Sandbox",
        .width  = rw::ui::default_window_width,
        .height = rw::ui::default_window_height,
    };

    SandboxApp() :
        rw::engine::App(window_settings), // example_layer_{ push_layer<ExampleLayer>() },
        sandbox_2d_{ push_layer<Sandbox2D>() } {
        RW_INFO("Welcome to Sandbox, powered by RedWolf.");
    }

    ~SandboxApp() override                   = default;
    SandboxApp(const SandboxApp&)            = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    SandboxApp(SandboxApp&&)                 = default;
    SandboxApp& operator=(SandboxApp&&)      = default;

 private:
    Sandbox2D* sandbox_2d_{ nullptr };
};

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<SandboxApp>();
}
