#include "redwolf/boot/entry_point.hpp"

#include "redwolf/engine.hpp"
#include "redwolf/logger.hpp"

#include <memory>

i32 main(i32 argc, const char** argv) {
    auto engine{ std::make_unique<rw::Engine>() };
    if (!engine->init()) {
        rw::fatal("Failed to initialise RedWolf.");
        return 1;
    }

    engine->loop();

    return 0;
}