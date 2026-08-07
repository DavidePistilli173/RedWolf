#pragma once

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
        /**
         * @brief Initialise the platform abstraction.
         * @param title Window title.
         * @return true if successfull, false otherwise.
         */
        [[nodiscard]] static bool init(std::string_view title);

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
         * @brief Handler for global object availability.
         */
        static void registry_handle_global_(void* data, wl_registry* registry, u32 name, const char* interface, u32 version);

        /**
         * @brief Handler for global object removal.
         */
        static void registry_handle_global_remove_(void* data, wl_registry* registry, u32 name);

        /**
         * @brief Setup the wayland global object registry.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_registry_();

        /**
         * @brief Setup XDG objects.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool setup_xdg_();

        /**
         * @brief Handler for configure events from the xdg_surface object.
         */
        static void xdg_surface_handle_configure_(void* data, xdg_surface* xdg_surface, u32 serial);

        /**
         * @brief Handler for the close events from the xdg_top_level object.
         */
        static void xdg_top_level_handle_close(void* data, xdg_toplevel* xdg_top_level);

        /**
         * @brief Handler for configure events from the xdg_top_level object.
         */
        static void xdg_top_level_handle_configure_(void* data, xdg_toplevel* xdg_top_level, i32 width, i32 height, wl_array* states);

        /**
         * @brief Handler for configure bounds events from the xdg_top_level object.
         */
        static void xdg_top_level_handle_configure_bounds_(void* data, xdg_toplevel* xdg_top_level, i32 width, i32 height);

        /**
         * @brief Handler for wm capaibilites events from the xdg_top_level object.
         */
        static void xdg_top_level_handle_wm_capabilities_(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities);

        /**
         * @brief Handler for ping events from the xdg_wm_base object.
         */
        static void xdg_wm_base_handle_ping_(void* data, xdg_wm_base* xdg_wm_base, u32 serial);

        std::string window_title_; /**< Window title. */

        wl_display*           display_{ nullptr };       /**< Wayland display. */
        wl_registry_listener  registry_listener_{};      /**< wl_registry event listener. */
        wl_registry*          registry_{ nullptr };      /**< Wayland object registry. */
        wl_compositor*        compositor_{ nullptr };    /**< Wayland compositor. */
        wl_surface*           surface_{ nullptr };       /**< Wayland surface to draw on. */
        xdg_wm_base*          xdg_wm_base_{ nullptr };   /**< XDG shell base object. */
        xdg_wm_base_listener  xdg_wm_base_listener_{};   /**< xdg_wm_base event listener. */
        xdg_surface*          xdg_surface_{ nullptr };   /**< XDG shell surface. */
        xdg_surface_listener  xdg_surface_listener_{};   /**< xdg_surface event listener. */
        xdg_toplevel*         xdg_top_level_{ nullptr }; /**< XDG top level surface. */
        xdg_toplevel_listener xdg_top_level_listener_{}; /**< xdg_top_level event listener. */

        bool resize_{ false }; /**< Signals that the window needs to be resized. */
        i32  new_width_{ 0 };  /**< New width requested by the compositor. */
        i32  new_height_{ 0 }; /**< New height requested by the compositor. */

        bool ready_to_resize_{ false }; /**< Signals that the window can actually be resized. */
        bool close_requested_{ false }; /**< Signals that the window should be closed. */
    };
} // namespace rw

#endif