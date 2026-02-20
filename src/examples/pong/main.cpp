//
// Created by david on 20/02/2026.
//

import pong.app;
import redwolf;

constexpr rw::ui::WindowDescriptor window_settings{
    .title  = "Pong",
    .width  = rw::ui::default_window_width,
    .height = rw::ui::default_window_height,
};

int main(int argc, char** argv) {
    rw::engine::Engine<PongApp> engine{ window_settings, argc, argv };
    engine.run();
    return 0;
}