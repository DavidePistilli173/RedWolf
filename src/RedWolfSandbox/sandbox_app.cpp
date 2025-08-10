//
// Created by cjm036653 on 10/08/25.
//

#include "sandbox_app.hpp"

#include <RedWolf/util/logger.hpp>

std::unique_ptr<rw::core::App> rw::core::create_app() {
    rw::util::Logger::get().info("Creating SandboxApp instance.");
    return std::make_unique<SandboxApp>();
}