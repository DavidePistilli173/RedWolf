//
// Created by cjm036653 on 13/08/25.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../evt/event.hpp"
#include "../util/logger.hpp"
#include "vendor/glfw.hpp"

#include <functional>
#include <string_view>

namespace rw::ui {
    static constexpr uint32_t default_window_width{ 1280U }; /**< Default width of the window in pixels. */
    static constexpr uint32_t default_window_height{ 720U }; /**< Default height of the window in pixels. */

    /**
     * @brief Descriptor for a window, containing properties like title, size, and other configurations.
     */
    struct WindowDescriptor {
        rw::util::Logger& logger;                          /**< Logger instance for logging messages. */
        std::string_view  title{ "RedWolf Engine" };       /**< Title of the window. */
        uint32_t          width{ default_window_width };   /**< Width of the window in pixels. */
        uint32_t          height{ default_window_height }; /**< Height of the window in pixels. */
    };

    /**
     * @brief Class for managing a graphical window.
     */
    class Window {
     public:
        /**
         * Constructor.
         * @param descriptor Window settings.
         */
        explicit Window(const WindowDescriptor& descriptor);

        /**
         * @brief Destructor.
         */
        ~Window();

        /**
         * @brief Copy constructor.
         */
        Window(const Window&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Window& operator=(const Window&) = delete;

        /**
         * @brief Move constructor.
         */
        Window(Window&& other) noexcept;

        /**
         * @brief Move-assignment operator.
         */
        Window& operator=(Window&& other) = delete;

        /**
         * @brief Get the current height of the window.
         * @return Current height of the window in pixels.
         */
        [[nodiscard]] uint32_t height() const;

        /**
         * @brief Update the window, poll events etc.
         * @details Called once per frame.
         */
        void update();

        /**
         * @brief Set the callback for handling window events.
         * @param callback Callback to call when an event occurs.
         */
        void set_event_callback(const std::function<void(const rw::evt::Event&)>& callback);

        /**
         * @brief Enable or disable vertical synchronization (VSync).
         * @param enabled If true, enable VSync; if false, disable it.
         */
        void set_vsync(const bool enabled);

        /**
         * @brief Get the current state of vertical synchronization (VSync).
         * @return Current state of VSync: true if enabled, false if disabled.
         */
        [[nodiscard]] bool vsync() const;

        /**
         * @brief Get the current width of the window.
         * @return Current width of the window in pixels.
         */
        [[nodiscard]] uint32_t width() const;

     private:
        /**
         * @brief Close the window, if it is initalised.
         */
        void close_();

        static bool glfw_initialized_; /**< Flag to check if GLFW has been initialized. */

        rw::util::Logger&                          logger_;                          /**< Logger instance for logging messages. */
        GLFWwindow*                                handle_{ nullptr };               /**< Raw window handle. */
        std::string                                title_{ "RedWolf Engine" };       /**< Title of the window. */
        uint32_t                                   width_{ default_window_width };   /**< Width of the window in pixels. */
        uint32_t                                   height_{ default_window_height }; /**< Height of the window in pixels. */
        bool                                       vsync_{ true };                   /**< Vertical synchronization (VSync) state. */
        std::function<void(const rw::evt::Event&)> event_callback_{ nullptr };       /**< Callback for handling events. */
    };
} // namespace rw::ui

#endif // WINDOW_HPP
