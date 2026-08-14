#pragma once

#include <wayland-client-protocol.h>
#ifdef linux

    #include "redwolf/common.hpp"
    #include "redwolf/input/keyboard.hpp"
    #include "redwolf/input/mouse.hpp"
    #include "xdg-shell.h"
    #include "xkbcommon/xkbcommon.h"

    #include <string>
    #include <string_view>
    #include <wayland-client.h>

namespace rw {
    /**
     * @brief Platform abstraction code and data.
     */
    class Platform {
     public:
        static constexpr usize text_buffer_size{ 16 }; /**< Size of the temporary text buffer. */

        ~Platform();

        Platform(const Platform&)            = delete;
        Platform& operator=(const Platform&) = delete;

        Platform(Platform&&)            = delete;
        Platform& operator=(Platform&&) = delete;

        /**
         * @brief Initialise the platform abstraction.
         * @param title Window title.
         * @return true if successfull, false otherwise.
         */
        [[nodiscard]] static bool init(std::string_view title);

        /**
         * @brief Poll events from the window system.
         */
        static void poll_events();

        /**
         * @brief Enable/disable the text mode for keyboard input.
         * @param enabled if true enables text mode, if false disables it.
         */
        static void set_text_mode(bool enabled);

        /**
         * @brief Shutdown the platform abstraction.
         */
        static void shutdown();

     private:
        Platform() = default;

        /**
         * @brief Connect to the wayland display.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool connect_to_display_();

        /**
         * @brief Create the surface to draw on.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool create_surface_();

        /**
         * @brief Handler for keyboard->enter events.
         */
        static void handle_keyboard_enter_(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface, wl_array* keys);

        /**
         * @brief Handler for keyboard->key events.
         */
        static void handle_keyboard_key_(void* data, wl_keyboard* keyboard, u32 serial, u32 time, u32 key, u32 state);

        /**
         * @brief Handler for keyboard->keymap events.
         */
        static void handle_keyboard_keymap_(void* data, wl_keyboard* keyboard, u32 format, i32 fd, u32 size);

        /**
         * @brief Handler for keyboard->leave events.
         */
        static void handle_keyboard_leave_(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface);

        /**
         * @brief Handler for keyboard->modifiers events.
         */
        static void handle_keyboard_modifiers_(
            void* data, wl_keyboard* keyboard, u32 serial, u32 mods_depressed, u32 mods_latched, u32 mods_locked, u32 group);

        /**
         * @brief Handler for keyboard->repeat_info events.
         */
        static void handle_repeat_info_(void* data, wl_keyboard* keyboard, i32 rate, i32 delay);

        /**
         * @brief Handler for pointer->axis events.
         */
        static void handle_pointer_axis_(void* data, wl_pointer* pointer, u32 time, u32 axis, wl_fixed_t value);

        /**
         * @brief Handler for pointer->axis_discrete events.
         */
        static void handle_pointer_axis_discrete_(void* data, wl_pointer* pointer, u32 axis, i32 discrete);

        /**
         * @brief Handler for pointer->axis_relative_direction events.
         */
        static void handle_pointer_axis_relative_direction_(void* data, wl_pointer* pointer, u32 axis, u32 direction);

        /**
         * @brief Handler for pointer->axis_source events.
         */
        static void handle_pointer_axis_source_(void* data, wl_pointer* pointer, u32 axis_source);

        /**
         * @brief Handler for pointer->axis_stop events.
         */
        static void handle_pointer_axis_stop_(void* data, wl_pointer* pointer, u32 time, u32 axis);

        /**
         * @brief Handler for pointer->axis_value120 events.
         */
        static void handle_pointer_axis_value120_(void* data, wl_pointer*, u32 axis, i32 value120);

        /**
         * @brief Handler for pointer->button events.
         */
        static void handle_pointer_button_(void* data, wl_pointer* pointer, u32 serial, u32 time, u32 button, u32 state);

        /**
         * @brief Handler for pointer->enter events.
         */
        static void handle_pointer_enter_(
            void* data, wl_pointer* pointer, u32 serial, wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y);

        /**
         * @brief Handler for pointer->frame events.
         */
        static void handle_pointer_frame_(void* data, wl_pointer* pointer);

        /**
         * @brief Handler for pointer->leave events.
         */
        static void handle_pointer_leave_(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface);

        /**
         * @brief Handler for pointer->motion events.
         */
        static void handle_pointer_motion_(void* data, wl_pointer* pointer, u32 time, wl_fixed_t surface_x, wl_fixed_t surface_y);

        /**
         * @brief Handler for pointer->warp events.
         */
        static void handle_pointer_warp_(void* data, wl_pointer* pointer, wl_fixed_t surface_x, wl_fixed_t surface_y);

        /**
         * @brief Handler for registry->global events.
         */
        static void handle_registry_global_(void* data, wl_registry* registry, u32 name, const char* interface, u32 version);

        /**
         * @brief Handler for registry->global_remove events.
         */
        static void handle_registry_global_remove_(void* data, wl_registry* registry, u32 name);

        /**
         * @brief Handler for seat->capabilities events.
         */
        static void handle_seat_capabilities_(void* data, wl_seat* seat, u32 capabilities);

        /**
         * @brief Handler for seat->name events.
         */
        static void handle_seat_name_(void* data, wl_seat* seat, const char* name);

        /**
         * @brief Handler for xdg_surface->configure events.
         */
        static void handle_xdg_surface_configure_(void* data, xdg_surface* xdg_surface, u32 serial);

        /**
         * @brief Handler for xdg_toplevel->close events.
         */
        static void handle_xdg_toplevel_close(void* data, xdg_toplevel* xdg_top_level);

        /**
         * @brief Handler for xdg_toplevel->configure events.
         */
        static void handle_xdg_toplevel_configure_(void* data, xdg_toplevel* xdg_top_level, i32 width, i32 height, wl_array* states);

        /**
         * @brief Handler for xdg_toplevel->configure_bounds events.
         */
        static void handle_xdg_toplevel_configure_bounds_(void* data, xdg_toplevel* xdg_top_level, i32 width, i32 height);

        /**
         * @brief Handler for xdg_toplevel->wm_capaibilites events.
         */
        static void handle_xdg_toplevel_wm_capabilities_(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities);

        /**
         * @brief Handler for xdg_wm_base->ping events.
         */
        static void handle_xdg_wm_base_ping_(void* data, xdg_wm_base* xdg_wm_base, u32 serial);

        /**
         * @brief Prepare all event listeners.
         */
        void prepare_listeners_();

        /**
         * @brief Process raw key input.
         * @param key wayland key scancode.
         * @param state pressed/released state.
         */
        void process_raw_key_(u32 key, u32 state);

        /**
         * @brief Process text key input.
         * @param key wayland key scancode.
         * @param state pressed/released state.
         */
        void process_text_key_(u32 key, u32 state);

        /**
         * @brief Setup keyboard input.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_keyboard_();

        /**
         * @brief Setup mouse pointer input.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_pointer_();

        /**
         * @brief Setup the wayland global object registry.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_registry_();

        /**
         * @brief Setup touch input.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_touch_();

        /**
         * @brief Translate a wayland keycode into an engine key.
         * @param scancode Key scancode received from wayland.
         * @return Equivalent engine key.
         */
        [[nodiscard]] static Key translate_keyboard_key_(u32 scancode);

        /**
         * @brief Translate a mouse button code from linux to engine.
         * @param linux_code Linux-specific code for the mouse button.
         * @return Equivalent engine code.
         */
        [[nodiscard]] static MouseBtn translate_mouse_btn_(u32 linux_code);

        std::string window_title_; /**< Window title. */

        wl_registry_listener  registry_listener_{};      /**< wl_registry event listener. */
        xdg_wm_base_listener  xdg_wm_base_listener_{};   /**< xdg_wm_base event listener. */
        xdg_surface_listener  xdg_surface_listener_{};   /**< xdg_surface event listener. */
        xdg_toplevel_listener xdg_top_level_listener_{}; /**< xdg_top_level event listener. */
        wl_seat_listener      seat_listener_{};          /**< wl_seat event listener. */
        wl_pointer_listener   pointer_listener_{};       /**< wl_pointer event listener. */
        wl_keyboard_listener  keyboard_listener_{};      /**< wl_keyboard event listener. */
        wl_touch_listener     touch_listener_{};         /**< wl_touch event listener. */

        wl_display*    display_{ nullptr };       /**< Wayland display. */
        wl_registry*   registry_{ nullptr };      /**< Wayland object registry. */
        wl_compositor* compositor_{ nullptr };    /**< Wayland compositor. */
        wl_surface*    surface_{ nullptr };       /**< Wayland surface to draw on. */
        xdg_wm_base*   xdg_wm_base_{ nullptr };   /**< XDG shell base object. */
        xdg_surface*   xdg_surface_{ nullptr };   /**< XDG shell surface. */
        xdg_toplevel*  xdg_top_level_{ nullptr }; /**< XDG top level surface. */

        bool resize_{ false }; /**< Signals that the window needs to be resized. */
        i32  new_width_{ 0 };  /**< New width requested by the compositor. */
        i32  new_height_{ 0 }; /**< New height requested by the compositor. */

        bool ready_to_resize_{ false }; /**< Signals that the window can actually be resized. */
        bool close_requested_{ false }; /**< Signals that the window should be closed. */

        wl_seat*     seat_{ nullptr };     /**< Global object containing input sub-objects. */
        wl_pointer*  pointer_{ nullptr };  /**< Mouse pointer object. */
        wl_keyboard* keyboard_{ nullptr }; /**< Keyboard object. */
        wl_touch*    touch_{ nullptr };    /**< Touch screen object. */

        f64 pointer_scroll_x_{ 0.0F }; /**< Horizontal mouse scroll since the last pointer->frame event. */
        f64 pointer_scroll_y_{ 0.0F }; /**< Horizontal mouse scroll since the last pointer->frame event. */

        bool                               text_mode_{ false };     /**< If true, keyboard input is processed as text. */
        std::array<char, text_buffer_size> text_buffer_{};          /**< Temporary buffer for input text. */
        xkb_context*                       xkb_context_{ nullptr }; /**< X keyboard context. */
        xkb_keymap*                        xkb_keymap_{ nullptr };  /**< X keyboard key mapping. */
        xkb_state*                         xkb_state_{ nullptr };   /**< X keyboard state. */
    };
} // namespace rw

#endif