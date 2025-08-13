//
// Created by cjm036653 on 10/08/25.
//

#include "sandbox_app.hpp"

#include <RedWolf/evt/application_event.hpp>
#include <RedWolf/util/logger.hpp>

SandboxApp::SandboxApp() {
    logger().info("Welcome to Sandbox, powered by RedWolf.");
}

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<SandboxApp>();
}