//
// Created by david on 21/12/2025.
//

#include "pong.hpp"

#include "common.hpp"

#include <RedWolf/util/logger.hpp>

Pong::Pong() :
    rw::engine::App(window_settings), // example_layer_{ push_layer<ExampleLayer>() },
    main_game_{ push_layer<MainGame>() } {
    RW_INFO("Welcome to Pong, powered by RedWolf.");
}

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<Pong>();
}