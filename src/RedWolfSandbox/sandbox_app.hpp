//
// Created by cjm036653 on 10/08/25.
//

#ifndef SANDBOX_APP_HPP
#define SANDBOX_APP_HPP

#include <RedWolf/engine/app.hpp>

class SandboxApp : public rw::engine::App {
 public:
    SandboxApp();
    ~SandboxApp() override = default;
    SandboxApp(const SandboxApp&) = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    SandboxApp(SandboxApp&&) = default;
    SandboxApp& operator=(SandboxApp&&) = default;
};

#endif // SANDBOX_APP_HPP
