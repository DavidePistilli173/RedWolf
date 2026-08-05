#include <iostream>
#include <redwolf/boot/entry_point.hpp>
#include <redwolf/logger.hpp>

void rw_user::init_modules() {
    rw::trace("Hello World!");
    rw::info("Hello World!");
    rw::warn("Hello World!");
    rw::error("Hello World!");
    rw::fatal("Hello World!");
}