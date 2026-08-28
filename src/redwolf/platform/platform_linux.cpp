#include "platform_linux.hpp"

#ifdef linux

    #include "platform_common.hpp"
    #include "redwolf/events/events.hpp"
    #include "redwolf/input/input.hpp"
    #include "redwolf/input/mouse.hpp"
    #include "redwolf/logger.hpp"
    #include "redwolf/profiler.hpp"
    #include "redwolf/user_data.hpp"
    #include "xdg-shell.h"

    #include <cstring>
    #include <linux/input-event-codes.h>
    #include <sys/mman.h>
    #include <unistd.h>
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

void rw::Platform::append_required_extension_names(Vec<const char*>& enabled_extensions) {
    (void) enabled_extensions.emplace_back("VK_KHR_wayland_surface");
}

wl_display* rw::Platform::display() {
    return g_platform->display_;
}

bool rw::Platform::init() {
    RW_PROFILE_SCOPE

    // Platform already initialised.
    if (nullptr != g_platform) {
        warn("Platform already initialised.");
        return true;
    }

    g_platform                = new Platform();
    g_platform->window_title_ = UserData::app_name();

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

void rw::Platform::set_text_mode(bool enabled) {
    if (nullptr == g_platform) {
        error("Platform not yet initialised.");
        return;
    }

    g_platform->text_mode_ = enabled;
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

wl_surface* rw::Platform::surface() {
    return g_platform->surface_;
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

void rw::Platform::handle_keyboard_enter_(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface, wl_array* keys) {
    u32*       key{ nullptr };
    const u32* begin{ static_cast<const u32*>(keys->data) };
    const u32* end{ begin + (keys->size / sizeof(u32)) };

    if (g_platform->text_mode_) {
        for (const u32* it{ begin }; it != end; ++it) {
            g_platform->process_text_key_(*it, WL_KEYBOARD_KEY_STATE_PRESSED);
        }
    } else {
        for (const u32* it{ begin }; it != end; ++it) {
            g_platform->process_raw_key_(*it, WL_KEYBOARD_KEY_STATE_PRESSED);
        }
    }
}

void rw::Platform::handle_keyboard_key_(void* data, wl_keyboard* keyboard, u32 serial, u32 time, u32 key, u32 state) {
    if (g_platform->text_mode_) {
        g_platform->process_text_key_(key, state);
    } else {
        g_platform->process_raw_key_(key, state);
    }
}

void rw::Platform::handle_keyboard_keymap_(void* data, wl_keyboard* keyboard, u32 format, i32 fd, u32 size) {
    if (WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1 != format) {
        error("Unsupported keymap format: '{}'", format);
        return;
    }

    // Map the keymap file descriptor into memory.
    char* map_shm{ reinterpret_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0)) };
    if (MAP_FAILED == map_shm) {
        error("Failed to map file descriptor '{}' into memory.", fd);
        return;
    }

    // Clear any state and keymap, if already present.
    xkb_state_unref(g_platform->xkb_state_);
    g_platform->xkb_state_ = nullptr;
    xkb_keymap_unref(g_platform->xkb_keymap_);
    g_platform->xkb_keymap_ = nullptr;

    g_platform->xkb_keymap_ =
        xkb_keymap_new_from_string(g_platform->xkb_context_, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

    munmap(map_shm, size);
    close(fd);

    if (nullptr == g_platform->xkb_keymap_) {
        error("Failed to create keymap.");
        return;
    }

    g_platform->xkb_state_ = xkb_state_new(g_platform->xkb_keymap_);
    if (nullptr == g_platform->xkb_state_) {
        error("Failed to create xkb state.");
    }
}

void rw::Platform::handle_keyboard_leave_(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface) {
    Input::reset_keyboard_keys();
}

void rw::Platform::handle_keyboard_modifiers_(
    void* data, wl_keyboard* keyboard, u32 serial, u32 mods_depressed, u32 mods_latched, u32 mods_locked, u32 group) {
    if (nullptr != g_platform->xkb_state_) {
        xkb_state_update_mask(g_platform->xkb_state_, mods_depressed, mods_latched, mods_locked, 0, 0, group);
    }
}

void rw::Platform::handle_repeat_info_(void* data, wl_keyboard* keyboard, i32 rate, i32 delay) {
    Input::update_keyboard_repeat_info(std::chrono::milliseconds(delay), rate);
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
        xkb_state_unref(g_platform->xkb_state_);
        g_platform->xkb_state_ = nullptr;

        xkb_keymap_unref(g_platform->xkb_keymap_);
        g_platform->xkb_keymap_ = nullptr;

        xkb_context_unref(g_platform->xkb_context_);
        g_platform->xkb_context_ = nullptr;

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

        if ((0 < g_platform->new_width_) && (0 < g_platform->new_height_)) {
            Events::fire(
                WindowResizeEvent{ .old_width  = static_cast<u32>(g_platform->old_width_),
                                   .old_height = static_cast<u32>(g_platform->old_height_),
                                   .new_width  = static_cast<u32>(g_platform->new_width_),
                                   .new_height = static_cast<u32>(g_platform->new_height_) });
            info("Window resize event fired with new size: {}x{}", g_platform->new_width_, g_platform->new_height_);
        }
    }
}

void rw::Platform::handle_xdg_toplevel_close([[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level) {
    RW_PROFILE_SCOPE

    g_platform->close_requested_ = true;
    Events::fire(WindowCloseEvent{});
    info("Window close event fired.");
}

void rw::Platform::handle_xdg_toplevel_configure_(
    [[maybe_unused]] void* data, [[maybe_unused]] xdg_toplevel* xdg_top_level, i32 width, i32 height, [[maybe_unused]] wl_array* states) {
    RW_PROFILE_SCOPE

    if ((0 != width) && (0 != height)) {
        g_platform->resize_     = true;
        g_platform->old_width_  = g_platform->new_width_;
        g_platform->old_height_ = g_platform->new_height_;
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

    g_platform->keyboard_listener_.enter       = &Platform::handle_keyboard_enter_;
    g_platform->keyboard_listener_.key         = &Platform::handle_keyboard_key_;
    g_platform->keyboard_listener_.keymap      = &Platform::handle_keyboard_keymap_;
    g_platform->keyboard_listener_.leave       = &Platform::handle_keyboard_leave_;
    g_platform->keyboard_listener_.modifiers   = &Platform::handle_keyboard_modifiers_;
    g_platform->keyboard_listener_.repeat_info = &Platform::handle_repeat_info_;
}

void rw::Platform::process_raw_key_(u32 key, u32 state) {
    const Key engine_key{ translate_keyboard_key_(key) };
    Input::update_keyboard_key(engine_key, WL_KEYBOARD_KEY_STATE_PRESSED == state);
}

void rw::Platform::process_text_key_(u32 key, u32 state) {
    const xkb_keycode_t xkb_code{ key + 8 };

    if (WL_KEYBOARD_KEY_STATE_PRESSED == state) {
        const usize len{ static_cast<usize>(xkb_state_key_get_utf8(xkb_state_, xkb_code, text_buffer_.data(), text_buffer_.size())) };
        if (text_buffer_.size() <= len) {
            warn("Temporary text buffer too small.");
        }

        if (0 < len) {
            Input::update_keyboard_text(std::string_view{ text_buffer_.data(), len });
        }
    }
}

bool rw::Platform::setup_keyboard_() {
    RW_PROFILE_SCOPE

    xkb_context_ = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (nullptr == xkb_context_) {
        error("Failed to create xkb context.");
        return false;
    }

    g_platform->keyboard_ = wl_seat_get_keyboard(seat_);
    if (nullptr == g_platform->keyboard_) {
        error("Null keyboard object.");
        return false;
    }

    if (0 != wl_keyboard_add_listener(g_platform->keyboard_, &(g_platform->keyboard_listener_), nullptr)) {
        error("Failed to add keyboard listener.");
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

rw::Key rw::Platform::translate_keyboard_key_(u32 scancode) {
    switch (scancode) {
    case KEY_ESC:
        return Key::esc;
    case KEY_1:
        return Key::num_1;
    case KEY_2:
        return Key::num_2;
    case KEY_3:
        return Key::num_3;
    case KEY_4:
        return Key::num_4;
    case KEY_5:
        return Key::num_5;
    case KEY_6:
        return Key::num_6;
    case KEY_7:
        return Key::num_7;
    case KEY_8:
        return Key::num_8;
    case KEY_9:
        return Key::num_9;
    case KEY_0:
        return Key::num_0;
    case KEY_MINUS:
        return Key::minus;
    case KEY_EQUAL:
        return Key::equal;
    case KEY_BACKSPACE:
        return Key::backspace;
    case KEY_TAB:
        return Key::tab;
    case KEY_Q:
        return Key::q;
    case KEY_W:
        return Key::w;
    case KEY_E:
        return Key::e;
    case KEY_R:
        return Key::r;
    case KEY_T:
        return Key::t;
    case KEY_Y:
        return Key::y;
    case KEY_U:
        return Key::u;
    case KEY_I:
        return Key::i;
    case KEY_O:
        return Key::o;
    case KEY_P:
        return Key::p;
    case KEY_LEFTBRACE:
        return Key::left_brace;
    case KEY_RIGHTBRACE:
        return Key::right_brace;
    case KEY_ENTER:
        return Key::enter;
    case KEY_LEFTCTRL:
        return Key::left_ctrl;
    case KEY_A:
        return Key::a;
    case KEY_S:
        return Key::s;
    case KEY_D:
        return Key::d;
    case KEY_F:
        return Key::f;
    case KEY_G:
        return Key::g;
    case KEY_H:
        return Key::h;
    case KEY_J:
        return Key::j;
    case KEY_K:
        return Key::k;
    case KEY_L:
        return Key::l;
    case KEY_SEMICOLON:
        return Key::semicolon;
    case KEY_APOSTROPHE:
        return Key::apostrophe;
    case KEY_GRAVE:
        return Key::grave;
    case KEY_LEFTSHIFT:
        return Key::left_shift;
    case KEY_BACKSLASH:
        return Key::backslash;
    case KEY_Z:
        return Key::z;
    case KEY_X:
        return Key::x;
    case KEY_C:
        return Key::c;
    case KEY_V:
        return Key::v;
    case KEY_B:
        return Key::b;
    case KEY_N:
        return Key::n;
    case KEY_M:
        return Key::m;
    case KEY_COMMA:
        return Key::comma;
    case KEY_DOT:
        return Key::dot;
    case KEY_SLASH:
        return Key::slash;
    case KEY_RIGHTSHIFT:
        return Key::right_shift;
    case KEY_KPASTERISK:
        return Key::kp_asterisk;
    case KEY_LEFTALT:
        return Key::left_alt;
    case KEY_SPACE:
        return Key::space;
    case KEY_CAPSLOCK:
        return Key::capslock;
    case KEY_F1:
        return Key::f1;
    case KEY_F2:
        return Key::f2;
    case KEY_F3:
        return Key::f3;
    case KEY_F4:
        return Key::f4;
    case KEY_F5:
        return Key::f5;
    case KEY_F6:
        return Key::f6;
    case KEY_F7:
        return Key::f7;
    case KEY_F8:
        return Key::f8;
    case KEY_F9:
        return Key::f9;
    case KEY_F10:
        return Key::f10;
    case KEY_NUMLOCK:
        return Key::num_lock;
    case KEY_SCROLLLOCK:
        return Key::scroll_lock;
    case KEY_KP7:
        return Key::kp_num_7;
    case KEY_KP8:
        return Key::kp_num_8;
    case KEY_KP9:
        return Key::kp_num_9;
    case KEY_KPMINUS:
        return Key::kp_minus;
    case KEY_KP4:
        return Key::kp_num_4;
    case KEY_KP5:
        return Key::kp_num_5;
    case KEY_KP6:
        return Key::kp_num_6;
    case KEY_KPPLUS:
        return Key::kp_plus;
    case KEY_KP1:
        return Key::kp_num_1;
    case KEY_KP2:
        return Key::kp_num_2;
    case KEY_KP3:
        return Key::kp_num_3;
    case KEY_KP0:
        return Key::kp_num_0;
    case KEY_KPDOT:
        return Key::kp_dot;
    case KEY_ZENKAKUHANKAKU:
        return Key::invalid;
    case KEY_102ND:
        return Key::key_102nd;
    case KEY_F11:
        return Key::f11;
    case KEY_F12:
        return Key::f12;
    case KEY_RO:
        return Key::ro;
    case KEY_KATAKANA:
        return Key::invalid;
    case KEY_HIRAGANA:
        return Key::invalid;
    case KEY_HENKAN:
        return Key::invalid;
    case KEY_KATAKANAHIRAGANA:
        return Key::invalid;
    case KEY_MUHENKAN:
        return Key::invalid;
    case KEY_KPJPCOMMA:
        return Key::kp_jpcomma;
    case KEY_KPENTER:
        return Key::kp_enter;
    case KEY_RIGHTCTRL:
        return Key::right_ctrl;
    case KEY_KPSLASH:
        return Key::kp_slash;
    case KEY_SYSRQ:
        return Key::invalid;
    case KEY_RIGHTALT:
        return Key::right_alt;
    case KEY_LINEFEED:
        return Key::invalid;
    case KEY_HOME:
        return Key::home;
    case KEY_UP:
        return Key::up;
    case KEY_PAGEUP:
        return Key::page_up;
    case KEY_LEFT:
        return Key::left;
    case KEY_RIGHT:
        return Key::right;
    case KEY_END:
        return Key::end;
    case KEY_DOWN:
        return Key::down;
    case KEY_PAGEDOWN:
        return Key::page_down;
    case KEY_INSERT:
        return Key::insert;
    case KEY_DELETE:
        return Key::del;
    case KEY_MACRO:
        return Key::invalid;
    case KEY_MUTE:
        return Key::invalid;
    case KEY_VOLUMEDOWN:
        return Key::invalid;
    case KEY_VOLUMEUP:
        return Key::invalid;
    case KEY_POWER:
        return Key::invalid;
    case KEY_KPEQUAL:
        return Key::kp_equal;
    case KEY_KPPLUSMINUS:
        return Key::kp_plus_minus;
    case KEY_PAUSE:
        return Key::pause;
    case KEY_SCALE:
        return Key::invalid;
    case KEY_KPCOMMA:
        return Key::kp_comma;
    case KEY_HANGEUL:
        return Key::invalid;
    case KEY_YEN:
        return Key::yen;
    case KEY_LEFTMETA:
        return Key::left_meta;
    case KEY_RIGHTMETA:
        return Key::right_meta;
    case KEY_COMPOSE:
        return Key::invalid;
    case KEY_STOP:
        return Key::invalid;
    case KEY_AGAIN:
        return Key::invalid;
    case KEY_PROPS:
        return Key::invalid;
    case KEY_UNDO:
        return Key::invalid;
    case KEY_FRONT:
        return Key::invalid;
    case KEY_COPY:
        return Key::invalid;
    case KEY_OPEN:
        return Key::invalid;
    case KEY_PASTE:
        return Key::invalid;
    case KEY_FIND:
        return Key::invalid;
    case KEY_CUT:
        return Key::invalid;
    case KEY_HELP:
        return Key::invalid;
    case KEY_MENU:
        return Key::menu;
    case KEY_CALC:
        return Key::invalid;
    case KEY_SETUP:
        return Key::invalid;
    case KEY_SLEEP:
        return Key::invalid;
    case KEY_WAKEUP:
        return Key::invalid;
    case KEY_FILE:
        return Key::invalid;
    case KEY_SENDFILE:
        return Key::invalid;
    case KEY_DELETEFILE:
        return Key::invalid;
    case KEY_XFER:
        return Key::invalid;
    case KEY_PROG1:
        return Key::invalid;
    case KEY_PROG2:
        return Key::invalid;
    case KEY_WWW:
        return Key::invalid;
    case KEY_MSDOS:
        return Key::invalid;
    case KEY_COFFEE:
        return Key::invalid;
    case KEY_ROTATE_DISPLAY:
        return Key::invalid;
    case KEY_CYCLEWINDOWS:
        return Key::invalid;
    case KEY_MAIL:
        return Key::invalid;
    case KEY_BOOKMARKS:
        return Key::invalid;
    case KEY_COMPUTER:
        return Key::invalid;
    case KEY_BACK:
        return Key::back;
    case KEY_FORWARD:
        return Key::forward;
    case KEY_CLOSECD:
        return Key::invalid;
    case KEY_EJECTCD:
        return Key::invalid;
    case KEY_EJECTCLOSECD:
        return Key::invalid;
    case KEY_NEXTSONG:
        return Key::next_song;
    case KEY_PLAYPAUSE:
        return Key::play_pause;
    case KEY_PREVIOUSSONG:
        return Key::prev_song;
    case KEY_STOPCD:
        return Key::invalid;
    case KEY_RECORD:
        return Key::invalid;
    case KEY_REWIND:
        return Key::invalid;
    case KEY_PHONE:
        return Key::invalid;
    case KEY_ISO:
        return Key::invalid;
    case KEY_CONFIG:
        return Key::invalid;
    case KEY_HOMEPAGE:
        return Key::invalid;
    case KEY_REFRESH:
        return Key::refresh;
    case KEY_EXIT:
        return Key::invalid;
    case KEY_MOVE:
        return Key::invalid;
    case KEY_EDIT:
        return Key::invalid;
    case KEY_SCROLLUP:
        return Key::scroll_up;
    case KEY_SCROLLDOWN:
        return Key::scroll_down;
    case KEY_KPLEFTPAREN:
        return Key::kp_left_paren;
    case KEY_KPRIGHTPAREN:
        return Key::kp_right_paren;
    case KEY_NEW:
        return Key::invalid;
    case KEY_REDO:
        return Key::invalid;
    case KEY_F13:
        return Key::f13;
    case KEY_F14:
        return Key::f14;
    case KEY_F15:
        return Key::f15;
    case KEY_F16:
        return Key::f16;
    case KEY_F17:
        return Key::f17;
    case KEY_F18:
        return Key::f18;
    case KEY_F19:
        return Key::f19;
    case KEY_F20:
        return Key::f20;
    case KEY_F21:
        return Key::f21;
    case KEY_F22:
        return Key::f22;
    case KEY_F23:
        return Key::f23;
    case KEY_F24:
        return Key::f24;
    case KEY_PLAYCD:
        return Key::invalid;
    case KEY_PAUSECD:
        return Key::invalid;
    case KEY_PROG3:
        return Key::invalid;
    case KEY_PROG4:
        return Key::invalid;
    case KEY_ALL_APPLICATIONS:
        return Key::invalid;
    case KEY_SUSPEND:
        return Key::invalid;
    case KEY_CLOSE:
        return Key::invalid;
    case KEY_PLAY:
        return Key::invalid;
    case KEY_FASTFORWARD:
        return Key::invalid;
    case KEY_BASSBOOST:
        return Key::invalid;
    case KEY_PRINT:
        return Key::invalid;
    case KEY_HP:
        return Key::invalid;
    case KEY_CAMERA:
        return Key::invalid;
    case KEY_SOUND:
        return Key::invalid;
    case KEY_QUESTION:
        return Key::invalid;
    case KEY_EMAIL:
        return Key::invalid;
    case KEY_CHAT:
        return Key::invalid;
    case KEY_SEARCH:
        return Key::invalid;
    case KEY_CONNECT:
        return Key::invalid;
    case KEY_FINANCE:
        return Key::invalid;
    case KEY_SPORT:
        return Key::invalid;
    case KEY_SHOP:
        return Key::invalid;
    case KEY_ALTERASE:
        return Key::invalid;
    case KEY_CANCEL:
        return Key::invalid;
    case KEY_BRIGHTNESSDOWN:
        return Key::invalid;
    case KEY_BRIGHTNESSUP:
        return Key::invalid;
    case KEY_MEDIA:
        return Key::invalid;
    case KEY_SWITCHVIDEOMODE:
        return Key::invalid;
    case KEY_KBDILLUMTOGGLE:
        return Key::invalid;
    case KEY_KBDILLUMDOWN:
        return Key::invalid;
    case KEY_KBDILLUMUP:
        return Key::invalid;
    case KEY_SEND:
        return Key::invalid;
    case KEY_REPLY:
        return Key::invalid;
    case KEY_FORWARDMAIL:
        return Key::invalid;
    case KEY_SAVE:
        return Key::invalid;
    case KEY_DOCUMENTS:
        return Key::invalid;
    case KEY_BATTERY:
        return Key::invalid;
    case KEY_BLUETOOTH:
        return Key::invalid;
    case KEY_WLAN:
        return Key::invalid;
    case KEY_UWB:
        return Key::invalid;
    case KEY_UNKNOWN:
        return Key::invalid;
    case KEY_VIDEO_NEXT:
        return Key::invalid;
    case KEY_VIDEO_PREV:
        return Key::invalid;
    case KEY_BRIGHTNESS_CYCLE:
        return Key::invalid;
    case KEY_BRIGHTNESS_AUTO:
        return Key::invalid;
    case KEY_DISPLAY_OFF:
        return Key::invalid;
    case KEY_WWAN:
        return Key::invalid;
    case KEY_RFKILL:
        return Key::invalid;
    case KEY_MICMUTE:
        return Key::invalid;
    default:
        return Key::invalid;
    }
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