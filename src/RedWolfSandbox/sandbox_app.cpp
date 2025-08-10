//
// Created by cjm036653 on 10/08/25.
//

#include "sandbox_app.hpp"

std::unique_ptr<rw::core::App> rw::core::create_app() {
    return std::make_unique<SandboxApp>();
}