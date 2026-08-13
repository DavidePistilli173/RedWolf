#include "generic_module.hpp"
#include "redwolf/memory/memory_pool.hpp"

#include <iostream>
#include <redwolf/boot/entry_point.hpp>
#include <redwolf/logger.hpp>

std::string_view rw_user::app_name() {
    return "RedWolfTester";
}

rw::Vec<rw::Ptr<rw::Module>> rw_user::create_modules() {
    rw::Vec<rw::Ptr<rw::Module>> result{ rw::MemoryType::modules };
    (void) result.emplace_back(rw::Memory::new_object<rwt::GenericModule>(rw::MemoryType::modules));
    return result;
}