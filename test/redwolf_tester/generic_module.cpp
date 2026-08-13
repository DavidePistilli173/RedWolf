#include "generic_module.hpp"

#include <redwolf/logger.hpp>

bool rwt::GenericModule::init() {
    rw::trace("GenericModule initialised.");
    return true;
}

std::string_view rwt::GenericModule::name() {
    return "GenericModule";
}

bool rwt::GenericModule::on_update(f32 delta_time) {
    rw::trace("on_update: '{}'s", delta_time);
}

bool rwt::GenericModule::on_render(f32 delta_time) {
    rw::trace("on_render: '{}'s", delta_time);
}