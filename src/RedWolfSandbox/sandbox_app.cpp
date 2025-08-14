//
// Created by cjm036653 on 10/08/25.
//

#include "sandbox_app.hpp"

#include <RedWolf/evt/application_event.hpp>
#include <RedWolf/ui/debug_layer.hpp>
#include <RedWolf/util/logger.hpp>

SandboxApp::SandboxApp() {
    RW_INFO("Welcome to Sandbox, powered by RedWolf.");

    debug_layer_id_ = push_overlay<rw::ui::DebugLayer>();
    RW_INFO("Debug layer created with ID {}", debug_layer_id_);
}

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<SandboxApp>();
}