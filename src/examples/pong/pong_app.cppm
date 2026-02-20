module;

#include <memory>

export module pong.app;

import pong.common;
import pong.main_layer;
import redwolf;

export class PongApp : public rw::engine::App {
 public:
    PongApp() : rw::engine::App(window_settings), main_game_{ push_layer<PongMainLayer>() } {
        rw::info("Welcome to Pong, powered by RedWolf {}.", rw::current_version);
    }
    ~PongApp() override                = default;
    PongApp(const PongApp&)            = delete;
    PongApp& operator=(const PongApp&) = delete;
    PongApp(PongApp&&)                 = delete;
    PongApp& operator=(PongApp&&)      = delete;

 private:
    PongMainLayer* main_game_{ nullptr };
};

std::unique_ptr<rw::engine::App> rw::engine::create_app() {
    return std::make_unique<PongApp>();
}
