#include "user_data.hpp"

#include "boot/entry_point.hpp"
#include "logger.hpp"

namespace {
    rw::UserData* g_user_data{ nullptr }; // User data manager instance.
}

const std::string& rw::UserData::app_name() {
    return g_user_data->app_name_;
}

const rw::VersionInfo& rw::UserData::app_version() {
    return g_user_data->app_version_;
}

bool rw::UserData::init() {
    if (nullptr != g_user_data) {
        warn("User data manager already initialised.");
        return true;
    }

    g_user_data               = new UserData();
    g_user_data->app_name_    = rw_user::app_name();
    g_user_data->app_version_ = rw_user::app_version();
    return true;
}

void rw::UserData::shutdown() {
    if (nullptr == g_user_data) {
        warn("User data manager already shut down.");
        return;
    }

    delete g_user_data;
    g_user_data = nullptr;
}