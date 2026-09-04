#include "generic_module.hpp"

bool rwt::GenericModule::init() {
    rw::trace("GenericModule initialised.");
    return true;
}

std::string_view rwt::GenericModule::name() {
    return "GenericModule";
}

void rwt::GenericModule::on_close() {
    rw::info("Closing GenericModule");
}

void rwt::GenericModule::on_update(f32 delta_time) {}

void rwt::GenericModule::on_render(f32 delta_time) {}