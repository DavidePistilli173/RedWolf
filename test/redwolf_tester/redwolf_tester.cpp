#include "generic_module.hpp"
#include "redwolf/common.hpp"
#include "redwolf/memory/generic_allocator.hpp"
#include "version_info.hpp"

#include <redwolf/boot/entry_point.hpp>
#include <redwolf/logger.hpp>

std::string_view rw_user::app_name() {
    return "RedWolfTester";
}

rw::VersionInfo rw_user::app_version() {
    return redwolf_tester::version;
}

rw::Vec<rw::Ptr<rw::Module>> rw_user::create_modules() {
    rw::Vec<rw::Ptr<rw::Module>> result{ rw::MemoryCategory::modules };
    (void) result.emplace_back(rw::Memory::new_object<rwt::GenericModule>(rw::MemoryCategory::modules));
    return result;
}