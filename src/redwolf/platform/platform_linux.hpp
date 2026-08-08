#pragma once

#include <wayland-client-protocol.h>
#ifdef linux

    #include "redwolf/common.hpp"
    #include "xdg-shell.h"

    #include <string>
    #include <string_view>
    #include <wayland-client.h>

namespace rw {
    /**
     * @brief Platform abstraction code and data.
     */
    class Platform {
     public:
        ~Platform() = default;

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
    };
} // namespace rw

#endif