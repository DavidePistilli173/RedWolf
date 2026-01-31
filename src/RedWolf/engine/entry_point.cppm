module;

#include <memory>

export module redwolf.engine.entry_point;

import redwolf.engine.app;

extern std::unique_ptr<rw::engine::App> rw::engine::create_app();

/**
 * @brief Entry point for any application using the RedWolf engine.
 */
export int main() {
    auto app = rw::engine::create_app();
    app->run();
    return 0;
}
