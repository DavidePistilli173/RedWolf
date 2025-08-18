//
// Created by cjm036653 on 10/08/25.
//

#ifndef SANDBOX_APP_HPP
#define SANDBOX_APP_HPP

#include <RedWolf/engine/app.hpp>

class SandboxApp : public rw::engine::App {
 public:
    static constexpr rw::ui::WindowDescriptor window_settings{ .title        = "RedWolf Sandbox",
                                                               .width        = rw::ui::default_window_width,
                                                               .height       = rw::ui::default_window_height,
                                                               .graphics_api = rw::gfx::Api::opengl };

    SandboxApp();
    ~SandboxApp() override                   = default;
    SandboxApp(const SandboxApp&)            = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    SandboxApp(SandboxApp&&)                 = default;
    SandboxApp& operator=(SandboxApp&&)      = default;

 private:
};

#endif // SANDBOX_APP_HPP
