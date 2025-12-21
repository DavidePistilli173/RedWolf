//
// Created by david on 21/12/2025.
//

#ifndef SRC_REDWOLF_PONG_HPP
#define SRC_REDWOLF_PONG_HPP

#include "main_game.hpp"

#include <RedWolf/engine/app.hpp>

class Pong : public rw::engine::App {
 public:
    Pong();
    ~Pong() override             = default;
    Pong(const Pong&)            = delete;
    Pong& operator=(const Pong&) = delete;
    Pong(Pong&&)                 = delete;
    Pong& operator=(Pong&&)      = delete;

 private:
    MainGame* main_game_{ nullptr };
};

#endif // SRC_REDWOLF_PONG_HPP
