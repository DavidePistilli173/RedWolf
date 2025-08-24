//
// Created by cjm036653 on 10/08/25.
//

#include "sandbox_app.hpp"

#include <RedWolf/util/logger.hpp>

SandboxApp::SandboxApp() : rw::engine::App(window_settings), example_layer_{ push_layer<ExampleLayer>() } {
    RW_INFO("Welcome to Sandbox, powered by RedWolf.");
}

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<SandboxApp>();
}