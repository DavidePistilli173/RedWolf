#include "platform_linux.hpp"

#include <wayland-util.h>

#ifdef linux

    #include "redwolf/input/input.hpp"
    #include "redwolf/input/mouse.hpp"
    #include "redwolf/logger.hpp"
    #include "redwolf/platform/xdg-shell.h"
    #include "redwolf/profiler.hpp"

    #include <cstring>
    #include <linux/input-event-codes.h>
    #include <wayland-client-core.h>
    #include <wayland-client-protocol.h>

namespace {
    rw::Platform* g_platform{ nullptr }; // Platform instance.
}

rw::Platform::~Platform() {
    if (nullptr != touch_) {
        wl_touch_release(touch_);
    }

    if (nullptr != keyboard_) {
        wl_keyboard_release(keyboard_);
    }

    if (nullptr != pointer_) {
        wl_pointer_release(pointer_);
    }

    wl_seat_release(seat_);
    xdg_toplevel_destroy(xdg_top_level_);
    xdg_surface_destroy(xdg_surface_);
    wl_surface_destroy(surface_);
    xdg_wm_base_destroy(xdg_wm_base_);
    wl_compositor_destroy(compositor_);
    wl_registry_destroy(registry_);
    wl_display_disconnect(display_);
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

    g_platform->prepare_listeners_();

    if (!g_platform->connect_to_display_()) {
        return false;
    }

    if (!g_platform->setup_registry_()) {
        return false;
    }

    if (!g_platform->create_surface_()) {
        return false;
    }

    if (nullptr == g_platform->seat_) {
        error("Null wl_seat.");
        return false;
    }

    return true;
}

void rw::Platform::Platform::poll_events() {
    wl_display_roundtrip(g_platform->display_);
}

void rw::Platform::shutdown() {
    RW_PROFILE_SCOPE

    if (nullptr == g_platform) {
        warn("Platform already shut down.");
        return;
    }

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

    // Check that the objects were successfully initialised.
    if (nullptr == xdg_wm_base_) {
        error("Null xdg_wm_base.");
        return false;
    }

    if (nullptr == compositor_) {
        error("Null wl_compositor.");
        return false;
    }

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

    if (0 != xdg_surface_add_listener(xdg_surface_, &xdg_surface_listener_, nullptr)) {
        error("Failed to add xdg_surface_listener");
        return false;
    }

    xdg_top_level_ = xdg_surface_get_toplevel(xdg_surface_);
    if (nullptr == xdg_top_level_) {
        error("Failed to get xdg_otp_level.");
        return false;
    }

    if (0 != xdg_toplevel_add_listener(xdg_top_level_, &xdg_top_level_listener_, nullptr)) {
        error("Failed to add xdg_top_level_listener.");
        return false;
    }

    xdg_toplevel_set_title(xdg_top_level_, window_title_.c_str());
    xdg_toplevel_set_app_id(xdg_top_level_, window_title_.c_str());

    wl_surface_commit(g_platform->surface_);
    if (-1 == wl_display_roundtrip(g_platform->display_)) {
        error("Failed to synchronise with the wayland server.");
        return false;
    }
    wl_surface_commit(g_platform->surface_);

    return true;
}

void rw::Platform::handle_pointer_axis_(void* data, wl_pointer* pointer, u32 time, u32 axis, wl_fixed_t value) {
    if (WL_POINTER_AXIS_HORIZONTAL_SCROLL == axis) {
        g_platform->pointer_scroll_x_ += wl_fixed_to_double(value);
    } else if (WL_POINTER_AXIS_VERTICAL_SCROLL == axis) {
        g_platform->pointer_scroll_y_ += wl_fixed_to_double(value);
    } else {
        warn("Unknown axis value: '{}'", axis);
    }
}

void rw::Platform::handle_pointer_axis_discrete_(void* data, wl_pointer* pointer, u32 axis, i32 discrete) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_axis_relative_direction_(void* data, wl_pointer* pointer, u32 axis, u32 direction) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_axis_source_(void* data, wl_pointer* pointer, u32 axis_source) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_axis_stop_(void* data, wl_pointer* pointer, u32 time, u32 axis) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_axis_value120_(void* data, wl_pointer*, u32 axis, i32 value120) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_button_(void* data, wl_pointer* pointer, u32 serial, u32 time, u32 button, u32 state) {
    Input::update_mouse_button(translate_mouse_btn_(button), WL_POINTER_BUTTON_STATE_PRESSED == state);
}

void rw::Platform::handle_pointer_enter_(
    void* data, wl_pointer* pointer, u32 serial, wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    Input::update_mouse_position(static_cast<f32>(wl_fixed_to_double(surface_x)), static_cast<f32>(wl_fixed_to_double(surface_y)));
}

void rw::Platform::handle_pointer_frame_(void* data, wl_pointer* pointer) {
    Input::update_mouse_scroll(
        static_cast<f32>(g_platform->pointer_scroll_x_ / static_cast<f64>(g_platform->new_width_)),
        static_cast<f32>(g_platform->pointer_scroll_y_ / static_cast<f64>(g_platform->new_height_)));
    g_platform->pointer_scroll_x_ = 0.0F;
    g_platform->pointer_scroll_y_ = 0.0F;
}

void rw::Platform::handle_pointer_leave_(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface) {
    // Nothing to do.
}

void rw::Platform::handle_pointer_motion_(void* data, wl_pointer* pointer, u32 time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    Input::update_mouse_position(static_cast<f32>(wl_fixed_to_double(surface_x)), static_cast<f32>(wl_fixed_to_double(surface_y)));
}

void rw::Platform::handle_pointer_warp_(void* data, wl_pointer* pointer, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    Input::update_mouse_position(static_cast<f32>(wl_fixed_to_double(surface_x)), static_cast<f32>(wl_fixed_to_double(surface_y)));
}

void rw::Platform::handle_registry_global_(
    [[maybe_unused]] void* data, wl_registry* registry, u32 name, const char* interface, u32 version) {
    RW_PROFILE_SCOPE

    if (0 == std::strcmp(interface, xdg_wm_base_interface.name)) {
        g_platform->xdg_wm_base_ = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));

        if (0 != xdg_wm_base_add_listener(g_platform->xdg_wm_base_, &g_platform->xdg_wm_base_listener_, nullptr)) {
            error("Failed to add xdg_wm_base_listener.");
        }
    } else if (0 == std::strcmp(interface, wl_compositor_interface.name)) {
        g_platform->compositor_ = reinterpret_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    } else if (0 == std::strcmp(interface, wl_seat_interface.name)) {
        g_platform->seat_ = reinterpret_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, version));

        if (0 != wl_seat_add_listener(g_platform->seat_, &g_platform->seat_listener_, nullptr)) {
            error("Failed to add wl_seat_listener.");
        }
    }
}

void rw::Platform::handle_registry_global_remove_(
    [[maybe_unused]] void* data, [[maybe_unused]] wl_registry* registry, [[maybe_unused]] u32 name) {
    RW_PROFILE_SCOPE
    trace("registry->global_remove event.");
}

void rw::Platform::handle_seat_capabilities_([[maybe_unused]] void* data, [[maybe_unused]] wl_seat* seat, u32 capabilities) {
    RW_PROFILE_SCOPE

    const bool pointer_available{ 0 != (capabilities & WL_SEAT_CAPABILITY_POINTER) };
    const bool keyboard_available{ 0 != (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) };
    const bool touch_available{ 0 != (capabilities & WL_SEAT_CAPABILITY_TOUCH) };

    // Pointer capability changes.
    if ((nullptr == g_platform->pointer_) && pointer_available) {
        (void) g_platform->setup_pointer_();
    } else if ((nullptr != g_platform->pointer_) && !pointer_available) {
        trace("Pointer capability removed.");
        wl_pointer_release(g_platform->pointer_);
        g_platform->pointer_ = nullptr;
    }

    // Keyboard capability changes.
    if ((nullptr == g_platform->keyboard_) && keyboard_available) {
        (void) g_platform->setup_keyboard_();
    } else if ((nullptr != g_platform->keyboard_) && !keyboard_available) {
        trace("Keyboard capability removed.");
        wl_keyboard_release(g_platform->keyboard_);
        g_platform->keyboard_ = nullptr;
    }

    // Touch capability changes.
    if ((nullptr == g_platform->touch_) && touch_available) {
        (void) g_platform->setup_touch_();
    } else if ((nullptr != g_platform->touch_) && !touch_available) {
        trace("Touch capability removed.");
        wl_touch_release(g_platform->touch_);
        g_platform->touch_ = nullptr;
    }
}

void rw::Platform::handle_seat_name_([[maybe_unused]] void* data, [[maybe_unused]] wl_seat* seat, const char* name) {
    RW_PROFILE_SCOPE
    trace("seat->name event (name: '{}').", name);
}

void rw::Platform::handle_xdg_surface_configure_([[maybe_unused]] void* data, xdg_surface* xdg_surface, u32 serial) {
    RW_PROFILE_SCOPE

    xdg_surface_ack_configure(xdg_surface, serial);
    if (g_platform->resize_) {
        g_platform->ready_to_resize_ = true;
    }
}

void rw::Platform::handle_xdg_toplevel_close([[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level) {
    RW_PROFILE_SCOPE

    g_platform->close_requested_ = true;
}

void rw::Platform::handle_xdg_toplevel_configure_(
    [[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level, i32 width, i32 height, [[maybe_unused]] wl_array* states) {
    RW_PROFILE_SCOPE

    if ((0 != width) && (0 != height)) {
        g_platform->resize_     = true;
        g_platform->new_width_  = width;
        g_platform->new_height_ = height;
    }
}

void rw::Platform::handle_xdg_toplevel_configure_bounds_(
    [[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level, i32 width, i32 height) {
    RW_PROFILE_SCOPE
    trace("xdg_toplevel->configure_bounds event. (width: '{}', height: '{}')", width, height);
}

void rw::Platform::handle_xdg_toplevel_wm_capabilities_(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities) {
    RW_PROFILE_SCOPE
    trace("xdg_toplevel->wm_capabilities event.");
}

void rw::Platform::handle_xdg_wm_base_ping_([[maybe_unused]] void* data, xdg_wm_base* xdg_wm_base, u32 serial) {
    RW_PROFILE_SCOPE

    xdg_wm_base_pong(xdg_wm_base, serial); // Reply to the xdg shell so that it knows the application is alive.
}

void rw::Platform::prepare_listeners_() {
    RW_PROFILE_SCOPE

    registry_listener_.global        = &Platform::handle_registry_global_;
    registry_listener_.global_remove = &Platform::handle_registry_global_remove_;

    g_platform->xdg_wm_base_listener_.ping = &Platform::handle_xdg_wm_base_ping_;

    xdg_surface_listener_.configure = &Platform::handle_xdg_surface_configure_;

    xdg_top_level_listener_.close            = &Platform::handle_xdg_toplevel_close;
    xdg_top_level_listener_.configure        = &Platform::handle_xdg_toplevel_configure_;
    xdg_top_level_listener_.configure_bounds = &Platform::handle_xdg_toplevel_configure_bounds_;
    xdg_top_level_listener_.wm_capabilities  = &Platform::handle_xdg_toplevel_wm_capabilities_;

    seat_listener_.capabilities = &Platform::handle_seat_capabilities_;
    seat_listener_.name         = &Platform::handle_seat_name_;
}

bool rw::Platform::setup_keyboard_() {
    RW_PROFILE_SCOPE

    g_platform->keyboard_ = wl_seat_get_keyboard(seat_);
    if (nullptr == g_platform->keyboard_) {
        error("Null keyboard object.");
        return false;
    }

    trace("Keyboard capability added.");
    return true;
}

bool rw::Platform::setup_pointer_() {
    RW_PROFILE_SCOPE

    g_platform->pointer_ = wl_seat_get_pointer(seat_);
    if (nullptr == g_platform->pointer_) {
        error("Null pointer object.");
        return false;
    }

    g_platform->pointer_listener_.axis                    = &Platform::handle_pointer_axis_;
    g_platform->pointer_listener_.axis_discrete           = &Platform::handle_pointer_axis_discrete_;
    g_platform->pointer_listener_.axis_relative_direction = &Platform::handle_pointer_axis_relative_direction_;
    g_platform->pointer_listener_.axis_source             = &Platform::handle_pointer_axis_source_;
    g_platform->pointer_listener_.axis_stop               = &Platform::handle_pointer_axis_stop_;
    g_platform->pointer_listener_.axis_value120           = &Platform::handle_pointer_axis_value120_;
    g_platform->pointer_listener_.button                  = &Platform::handle_pointer_button_;
    g_platform->pointer_listener_.enter                   = &Platform::handle_pointer_enter_;
    g_platform->pointer_listener_.frame                   = &Platform::handle_pointer_frame_;
    g_platform->pointer_listener_.leave                   = &Platform::handle_pointer_leave_;
    g_platform->pointer_listener_.motion                  = &Platform::handle_pointer_motion_;
    g_platform->pointer_listener_.warp                    = &Platform::handle_pointer_warp_;
    if (0 != wl_pointer_add_listener(g_platform->pointer_, &(g_platform->pointer_listener_), nullptr)) {
        error("Failed to add pointer_listener.");
        return false;
    }

    trace("Pointer capability added.");
    return true;
}

bool rw::Platform::setup_registry_() {
    RW_PROFILE_SCOPE

    // Setup the registry and registry listener.
    registry_ = wl_display_get_registry(display_);
    if (nullptr == registry_) {
        error("Failed to retrieve wl_registry.");
        return false;
    }

    if (0 != wl_registry_add_listener(registry_, &registry_listener_, nullptr)) {
        error("Failed to add wl_registry listener.");
        return false;
    }

    // Synchronise with the server.
    if (-1 == wl_display_roundtrip(display_)) {
        error("Failed to synchronise with wayland server.");
        return false;
    }

    return true;
}

bool rw::Platform::setup_touch_() {
    RW_PROFILE_SCOPE

    g_platform->touch_ = wl_seat_get_touch(seat_);
    if (nullptr == g_platform->touch_) {
        error("Null touch object.");
        return false;
    }

    trace("Touch capability added.");
    return true;
}

rw::MouseBtn rw::Platform::translate_mouse_btn_(u32 linux_code) {
    switch (linux_code) {
    case BTN_LEFT:
        return MouseBtn::left;
    case BTN_RIGHT:
        return MouseBtn::right;
    case BTN_MIDDLE:
        return MouseBtn::middle;
    case BTN_SIDE:
        return MouseBtn::side;
    case BTN_EXTRA:
        return MouseBtn::extra;
    case BTN_FORWARD:
        return MouseBtn::forward;
    case BTN_BACK:
        return MouseBtn::back;
    case BTN_TASK:
        return MouseBtn::task;
    default:
        return MouseBtn::invalid;
    }
}

#endif