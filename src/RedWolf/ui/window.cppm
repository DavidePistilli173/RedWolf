module;

#include "vendor/glfw.hpp"

#include <functional>
#include <glfw/glfw3.h>
#include <memory>
#include <string>
#include <string_view>

export module redwolf.ui.window;

import redwolf.evt.application_event;
import redwolf.evt.event;
import redwolf.evt.key_event;
import redwolf.evt.mouse_event;
import redwolf.gfx.context;
import redwolf.gfx.renderer_2_d;
import redwolf.input.keyboard;
import redwolf.core.math;
import redwolf.ui.common;
import redwolf.util.logger;

export namespace rw::ui {
    constexpr uint32_t default_window_width{ 1280U }; /**< Default width of the window in pixels. */
    constexpr uint32_t default_window_height{ 720U }; /**< Default height of the window in pixels. */

    /**
     * @brief Descriptor for a window, containing properties like title, size, and other configurations.
     */
    struct WindowDescriptor {
        std::string_view title{ "RedWolf Engine" };       /**< Title of the window. */
        uint32_t         width{ default_window_width };   /**< Width of the window in pixels. */
        uint32_t         height{ default_window_height }; /**< Height of the window in pixels. */
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
        explicit Window(const WindowDescriptor& descriptor) :
            title_{ descriptor.title }, width_{ descriptor.width }, height_{ descriptor.height } {
            // Initialize GLFW
            if (!glfw_initialized_) {
                if (GLFW_TRUE != glfwInit()) {
                    rw::fatal("Failed to initialise GLFW: {}", rw::vendor::glfw_get_error());
                    return;
                }

                glfwSetErrorCallback(glfw_error_clbk_);
                rw::info("GLFW initialised successfully");
                glfw_initialized_ = true;
            }

            // Create the window
            handle_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str(), nullptr, nullptr);
            if (nullptr == handle_) {
                rw::err("Failed to create window: {}", rw::vendor::glfw_get_error());
                return;
            }

            // Create and initialise the graphics Context.
            graphics_context_ = std::make_unique<rw::gfx::Context>(handle_);
            if (!graphics_context_->init()) {
                rw::err("Failed to initialise the graphics Context.");
                return;
            }

            glfwSetWindowUserPointer(handle_, this);
            set_vsync(true);

            renderer_2d_ = std::make_shared<rw::gfx::Renderer2D>();

            init_callbacks_();
        }

        /**
         * @brief Destructor.
         */
        ~Window() {
            close_();
        }

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
        Window(Window&& other) noexcept :
            handle_{ other.handle_ }, title_{ std::move(other.title_) }, width_{ other.width_ }, height_{ other.height_ },
            vsync_{ other.vsync_ }, event_callback_{ std::move(other.event_callback_) } {
            other.handle_ = nullptr; // Transfer ownership
        }

        /**
         * @brief Move-assignment operator.
         */
        Window& operator=(Window&& other) = delete;

        /**
         * @brief Get the raw window handle.
         * @return Raw window handle.
         */
        [[nodiscard]] rw::ui::WindowHandle handle() {
            return handle_;
        }

        /**
         * @brief Get the current height of the window.
         * @return Current height of the window in pixels.
         */
        [[nodiscard]] uint32_t height() const {
            return height_;
        }

        /**
         * @brief Get an interface to this window's 2D renderer.
         * @return Interface to this window's 2D renderer.
         */
        [[nodiscard]] std::shared_ptr<rw::gfx::Renderer2D>& renderer() {
            return renderer_2d_;
        }

        /**
         * @brief Set the callback for handling window events.
         * @param callback Callback to call when an event occurs. Must return true if the event was handled and false otherwise.
         */
        void set_event_callback(const std::function<bool(const rw::evt::Event&)>& callback) {
            event_callback_ = callback;
        }

        /**
         * @brief Enable or disable vertical synchronization (VSync).
         * @param enabled If true, enable VSync; if false, disable it.
         */
        void set_vsync(const bool enabled) {
            vsync_ = enabled;

            if (vsync_) {
                glfwSwapInterval(1); // Enable VSync
                rw::info("VSync enabled");
            } else {
                glfwSwapInterval(0); // Disable VSync
                rw::info("VSync disabled");
            }
        }

        /**
         * @brief Update the window, poll events etc.
         * @details Called once per frame.
         */
        void update() {
            glfwPollEvents();
            graphics_context_->swap_buffers();
        }

        /**
         * @brief Get the current state of vertical synchronization (VSync).
         * @return Current state of VSync: true if enabled, false if disabled.
         */
        [[nodiscard]] bool vsync() const {
            return vsync_;
        }

        /**
         * @brief Get the current width of the window.
         * @return Current width of the window in pixels.
         */
        [[nodiscard]] uint32_t width() const {
            return width_;
        }

     private:
        /**
         * @brief Callback called when scrolling is performed.
         * @param window Handle of the window that received the event.
         * @param character Typed character.
         */
        static void char_clbk_(rw::ui::WindowHandle window, unsigned int character) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            const auto* self{ static_cast<Window*>(user_ptr) };

            const rw::evt::KeyTypedEvent event{ static_cast<rw::input::Key>(character) };
            if (nullptr != self->event_callback_) {
                (void) self->event_callback_(event);
            }
        }

        /**
         * @brief Close the window, if it is initalised.
         */
        void close_() {
            if (nullptr != handle_) {
                glfwDestroyWindow(handle_);
                handle_ = nullptr;
            }
        }

        /**
         * @brief Callback called when the cursor moves.
         * @param window Handle to the window that generated the event.
         * @param x New x coordinate of the cursor, relative to the left edge of the window.
         * @param y New y coordinate of the cursor, relative to the top edge of the window.
         */
        static void cursor_position_clbk_(rw::ui::WindowHandle window, double x, double y) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            const auto* self{ static_cast<Window*>(user_ptr) };

            const rw::evt::MouseMovedEvent event{ x, y };
            if (nullptr != self->event_callback_) {
                (void) self->event_callback_(event);
            }
        }

        /**
         * @brief Callback for all GLFW errors.
         * @param code Error code.
         * @param description Error description.
         */
        static void glfw_error_clbk_(int code, const char* description) {
            rw::err("GLFW error: code: {}; description: {}", static_cast<rw::vendor::GlfwError>(code), description);
        }

        /**
         * @brief Initialise GLFW event callbacks.
         */
        void init_callbacks_() {
            glfwSetWindowCloseCallback(handle_, &Window::window_close_clbk_);
            glfwSetWindowSizeCallback(handle_, &Window::window_resize_clbk_);
            glfwSetCharCallback(handle_, &Window::char_clbk_);
            glfwSetKeyCallback(handle_, &Window::key_clbk_);
            glfwSetMouseButtonCallback(handle_, &Window::mouse_button_clbk_);
            glfwSetScrollCallback(handle_, &Window::scroll_clbk_);
            glfwSetCursorPosCallback(handle_, &Window::cursor_position_clbk_);
        }

        /**
         * @brief Callback called when a keyboard key is pressed or released.
         * TODO: Update the event to take the modifiers into account.
         * @param window Handle of the window that received the event.
         * @param key The keyboard key that was pressed or released.
         * @param scancode The platform-specific scancode of the key.
         * @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
         * @param mods Bit field describing which modifier keys were held down.
         */
        static void key_clbk_(rw::ui::WindowHandle window, int key, int scancode, int action, int mods) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            const auto* self{ static_cast<Window*>(user_ptr) };

            switch (static_cast<rw::input::KeyState>(action)) {
            case rw::input::KeyState::pressed: {
                const rw::evt::KeyPressedEvent event{ static_cast<rw::input::Key>(key), false };
                if (nullptr != self->event_callback_) {
                    (void) self->event_callback_(event);
                }
            } break;
            case rw::input::KeyState::released: {
                const rw::evt::KeyReleasedEvent event{ static_cast<rw::input::Key>(key) };
                if (nullptr != self->event_callback_) {
                    (void) self->event_callback_(event);
                }
            } break;
            case rw::input::KeyState::repeated: {
                const rw::evt::KeyPressedEvent event{ static_cast<rw::input::Key>(key), true };
                if (nullptr != self->event_callback_) {
                    (void) self->event_callback_(event);
                }
            } break;
            default:
                rw::err("Invalid GLFW action: {}", action);
                break;
            }
        }

        /**
         * @brief Callback called when a mouse button is pressed or released.
         * TODO: Update the event to take the modifiers into account.
         * @param window Handle of the window that received the event.
         * @param button Mouse button that was pressed or released.
         * @param action `GLFW_PRESS` or `GLFW_RELEASE`.
         * @param mods Bit field describing which modifier keys were held down.
         */
        static void mouse_button_clbk_(rw::ui::WindowHandle window, int button, int action, int mods) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            const auto* self{ static_cast<Window*>(user_ptr) };

            switch (static_cast<rw::input::KeyState>(action)) {
            case rw::input::KeyState::pressed: {
                const rw::evt::MouseButtonPressedEvent event{ static_cast<rw::input::MouseButton>(button) };
                if (nullptr != self->event_callback_) {
                    (void) self->event_callback_(event);
                }
            } break;
            case rw::input::KeyState::released: {
                const rw::evt::MouseButtonReleasedEvent event{ static_cast<rw::input::MouseButton>(button) };
                if (nullptr != self->event_callback_) {
                    (void) self->event_callback_(event);
                }
            } break;
            default:
                rw::err("Invalid GLFW action: {}", action);
                break;
            }
        }

        /**
         * @brief Callback called when scrolling is performed.
         * @param window Handle of the window that received the event.
         * @param x_offset Horizontal scrolling amount.
         * @param y_offset Vertical scrolling amount.
         */
        static void scroll_clbk_(rw::ui::WindowHandle window, double x_offset, double y_offset) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            auto* self{ static_cast<Window*>(user_ptr) };

            const rw::evt::MouseScrolledEvent event{ x_offset, y_offset };
            if (nullptr != self->event_callback_) {
                (void) self->event_callback_(event);
            }
        }

        /**
         * @brief Callback called when the window is closed.
         * @param window Handle of the window that was closed.
         */
        static void window_close_clbk_(rw::ui::WindowHandle window) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            auto* self{ static_cast<Window*>(user_ptr) };

            const rw::evt::WindowCloseEvent event{};
            if (nullptr != self->event_callback_) {
                (void) self->event_callback_(event);
            }
        }

        /**
         * @brief Callback called when the window gets resized.
         * @param window Handle of the window that was resized.
         * @param width New width of the window in pixels.
         * @param height New height of the window in pixels.
         */
        static void window_resize_clbk_(rw::ui::WindowHandle window, int width, int height) {
            void* user_ptr{ glfwGetWindowUserPointer(window) };
            if (nullptr == user_ptr) {
                return;
            }
            auto* self{ static_cast<Window*>(user_ptr) };

            if (width < 0 || height < 0) {
                rw::warn("Invalid window size: {}x{}", width, height);
                return;
            }
            const rw::evt::WindowResizedEvent event{ static_cast<uint32_t>(width),
                                                     static_cast<uint32_t>(height),
                                                     rw::core::Vec2{ static_cast<float>(width) / static_cast<float>(self->width_),
                                                                     static_cast<float>(height) / static_cast<float>(self->height_) } };

            self->width_  = event.width;
            self->height_ = event.height;

            if (nullptr == self->event_callback_) {
                return;
            }
            (void) self->event_callback_(event);
        }

        static bool glfw_initialized_; /**< Flag to check if GLFW has been initialized. */

        rw::ui::WindowHandle                       handle_{ invalid_window_handle }; /**< Raw window handle. */
        std::shared_ptr<rw::gfx::Renderer2D>       renderer_2d_;                     /**< Exclusively 2D renderer. */
        std::unique_ptr<rw::gfx::Context>          graphics_context_{ nullptr };     /**< Rendering Context. */
        std::string                                title_{ "RedWolf Engine" };       /**< Title of the window. */
        uint32_t                                   width_{ default_window_width };   /**< Width of the window in pixels. */
        uint32_t                                   height_{ default_window_height }; /**< Height of the window in pixels. */
        bool                                       vsync_{ true };                   /**< Vertical synchronization (VSync) state. */
        std::function<bool(const rw::evt::Event&)> event_callback_{ nullptr };       /**< Callback for handling events. */
    };
} // namespace rw::ui

bool rw::ui::Window::glfw_initialized_{ false };
