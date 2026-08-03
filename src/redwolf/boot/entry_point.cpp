#include "redwolf/boot/entry_point.hpp"

#include "redwolf/engine.hpp"

#include <memory>

i32 main(i32 argc, const char** argv) {
    auto engine{ std::make_unique<rw::Engine>() };

    rw_user::init_modules();

    return 0;
}