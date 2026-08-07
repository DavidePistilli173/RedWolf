#include "platform_linux.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/platform/xdg-shell.h"
#include "redwolf/profiler.hpp"

#include <cstring>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#ifdef linux

namespace {
    rw::Platform* g_platform{ nullptr }; // Platform instance.
}

bool rw::Platform::init(std::string_view title) {
    RW_PROFILE_SCOPE

    // Platform already initialised.
    if (nullptr != g_platform) {
        warn("Platform already initialised.");
        return true;
    }

    g_platform                = new Platform();
    g_platform->window_title_ = title;

    if (!g_platform->connect_to_display_()) {
        return false;
    }

    if (!g_platform->setup_registry_()) {
        return false;
    }

    if (!g_platform->setup_xdg_()) {
        return false;
    }

    if (!g_platform->create_surface_()) {
        return false;
    }

    wl_surface_commit(g_platform->surface_);
    if (-1 == wl_display_roundtrip(g_platform->display_)) {
        error("Failed to synchronise with the wayland server.");
        return false;
    }
    wl_surface_commit(g_platform->surface_);

    return true;
}

void rw::Platform::shutdown() {
    RW_PROFILE_SCOPE

    if (nullptr == g_platform) {
        warn("Platform already shut down.");
        return;
    }

    // Disconnect from the wayland display.
    wl_display_disconnect(g_platform->display_);

    delete g_platform;
    g_platform = nullptr;
}

bool rw::Platform::connect_to_display_() {
    RW_PROFILE_SCOPE

    display_ = wl_display_connect(nullptr);
    if (nullptr == display_) {
        error("Failed to connect to Wayland display.");
        return false;
    }
    return true;
}

bool rw::Platform::create_surface_() {
    RW_PROFILE_SCOPE

    surface_ = wl_compositor_create_surface(compositor_);
    if (nullptr == surface_) {
        error("Failed to create wl_surface.");
        return false;
    }

    xdg_surface_ = xdg_wm_base_get_xdg_surface(xdg_wm_base_, surface_);
    if (nullptr == xdg_surface_) {
        error("Failed to create xdg_surface.");
        return false;
    }

    xdg_surface_listener_.configure = &Platform::xdg_surface_handle_configure_;
    if (0 != xdg_surface_add_listener(xdg_surface_, &xdg_surface_listener_, nullptr)) {
        error("Failed to add xdg_surface_listener");
        return false;
    }

    xdg_top_level_ = xdg_surface_get_toplevel(xdg_surface_);
    if (nullptr == xdg_top_level_) {
        error("Failed to get xdg_top_level.");
        return false;
    }

    xdg_top_level_listener_.close            = &Platform::xdg_top_level_handle_close;
    xdg_top_level_listener_.configure        = &Platform::xdg_top_level_handle_configure_;
    xdg_top_level_listener_.configure_bounds = &Platform::xdg_top_level_handle_configure_bounds_;
    xdg_top_level_listener_.wm_capabilities  = &Platform::xdg_top_level_handle_wm_capabilities_;
    if (0 != xdg_toplevel_add_listener(xdg_top_level_, &xdg_top_level_listener_, nullptr)) {
        error("Failed to add xdg_top_level_listener.");
        return false;
    }

    xdg_toplevel_set_title(xdg_top_level_, window_title_.c_str());
    xdg_toplevel_set_app_id(xdg_top_level_, window_title_.c_str());

    return true;
}

void rw::Platform::registry_handle_global_(
    [[maybe_unused]] void* data, wl_registry* registry, u32 name, const char* interface, u32 version) {
    RW_PROFILE_SCOPE

    if (0 == std::strcmp(interface, xdg_wm_base_interface.name)) {
        g_platform->xdg_wm_base_ = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
    } else if (0 == std::strcmp(interface, wl_compositor_interface.name)) {
        g_platform->compositor_ = reinterpret_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    }
}

void rw::Platform::registry_handle_global_remove_(void* data, wl_registry* registry, u32 name) {
    RW_PROFILE_SCOPE
    // Nothing to do.
}

bool rw::Platform::setup_registry_() {
    RW_PROFILE_SCOPE

    // Setup the registry and registry listener.
    registry_ = wl_display_get_registry(display_);
    if (nullptr == registry_) {
        error("Failed to retrieve wl_registry.");
        return false;
    }

    registry_listener_.global        = &Platform::registry_handle_global_;
    registry_listener_.global_remove = &Platform::registry_handle_global_remove_;
    if (0 != wl_registry_add_listener(registry_, &registry_listener_, nullptr)) {
        error("Failed to add wl_registry listener.");
        return false;
    }

    // Synchronise with the server.
    if (-1 == wl_display_roundtrip(display_)) {
        error("Failed to synchronise with wayland server.");
        return false;
    }

    // Check that the objects were successfully initialised.
    if (nullptr == xdg_wm_base_) {
        error("Failed to bind xdg_wm_base.");
        return false;
    }

    if (nullptr == compositor_) {
        error("Failed to bind wl_compositor.");
        return false;
    }

    return true;
}

bool rw::Platform::setup_xdg_() {
    RW_PROFILE_SCOPE

    xdg_wm_base_listener_.ping = &Platform::xdg_wm_base_handle_ping_;
    if (0 != xdg_wm_base_add_listener(xdg_wm_base_, &xdg_wm_base_listener_, nullptr)) {
        error("Failed to add xdg_wm_base_listener.");
        return false;
    }
    return true;
}

void rw::Platform::xdg_surface_handle_configure_([[maybe_unused]] void* data, xdg_surface* xdg_surface, u32 serial) {
    RW_PROFILE_SCOPE

    xdg_surface_ack_configure(xdg_surface, serial);
    if (g_platform->resize_) {
        g_platform->ready_to_resize_ = true;
    }
}

void rw::Platform::xdg_top_level_handle_close([[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level) {
    RW_PROFILE_SCOPE

    g_platform->close_requested_ = true;
}

void rw::Platform::xdg_top_level_handle_configure_(
    [[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level, i32 width, i32 height, [[maybe_unused]] wl_array* states) {
    RW_PROFILE_SCOPE

    if ((0 != width) && (0 != height)) {
        g_platform->resize_     = true;
        g_platform->new_width_  = width;
        g_platform->new_height_ = height;
    }
}

void rw::Platform::xdg_top_level_handle_configure_bounds_(void* data, xdg_toplevel* xdg_top_level, i32 width, i32 height) {
    RW_PROFILE_SCOPE

    // Nothing to do.
}

void rw::Platform::xdg_top_level_handle_wm_capabilities_(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities) {
    RW_PROFILE_SCOPE

    // Nothing to do.
}

void rw::Platform::xdg_wm_base_handle_ping_([[maybe_unused]] void* data, xdg_wm_base* xdg_wm_base, u32 serial) {
    RW_PROFILE_SCOPE

    xdg_wm_base_pong(xdg_wm_base, serial); // Reply to the xdg shell so that it knows the application is alive.
}

#endif