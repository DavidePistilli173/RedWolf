module;

export module pong.common;

import redwolf;

export constexpr rw::ui::WindowDescriptor window_settings{
    .title  = "Pong",
    .width  = rw::ui::default_window_width,
    .height = rw::ui::default_window_height,
};
