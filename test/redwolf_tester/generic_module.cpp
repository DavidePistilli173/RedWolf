#include "generic_module.hpp"

bool rwt::GenericModule::init() {
    rw::trace("GenericModule initialised.");
    return true;
}

rw::View<char> rwt::GenericModule::name() {
    return "GenericModule";
}

void rwt::GenericModule::on_close() {
    rw::info("Closing GenericModule");
}

void rwt::GenericModule::on_update(f32 delta_time) {}

void rwt::GenericModule::on_render(f32 delta_time) {}