module;

#include <cstdint>
#include <string>

export module redwolf.evt.application_event;

import redwolf.evt.event;
import redwolf.core.math;

export namespace rw::evt {
    /**
     * @brief Window resizing event.
     */
    struct WindowResizedEvent final : Event {
        /**
         * @brief Constructor.
         * @param new_width New window width.
         * @param new_height New window height.
         * @param p_scale_factor Scale factor (new over old).
         */
        WindowResizedEvent(const uint32_t new_width, const uint32_t new_height, const rw::core::Vec2 p_scale_factor) :
            Event(), width{ new_width }, height{ new_height }, scale_factor{ p_scale_factor } {}

        /**
         * @brief Destructor.
         */
        ~WindowResizedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        WindowResizedEvent(const WindowResizedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        WindowResizedEvent& operator=(const WindowResizedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        WindowResizedEvent(WindowResizedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        WindowResizedEvent& operator=(WindowResizedEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<EventCategoryType::application>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "WindowResizedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::string("WindowResizedEvent{ width: ") + std::to_string(width) + "; height: " + std::to_string(height) + '}';
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::window_resize;
        }

        uint32_t       width{ 0 };           /**< New window width. */
        uint32_t       height{ 0 };          /**< New window height. */
        rw::core::Vec2 scale_factor{ 1.0F }; /**< Scale factor (new over old). */
    };

    /**
     * @brief Event for when the window closes.
     */
    struct WindowCloseEvent final : Event {
        /**
         * @brief Constructor.
         */
        WindowCloseEvent() = default;

        /**
         * @brief Destructor.
         */
        ~WindowCloseEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        WindowCloseEvent(const WindowCloseEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        WindowCloseEvent& operator=(const WindowCloseEvent&) = default;

        /**
         * @brief Move constructor.
         */
        WindowCloseEvent(WindowCloseEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        WindowCloseEvent& operator=(WindowCloseEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<EventCategoryType::application>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "WindowCloseEvent";
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return rw::evt::EventType::window_close;
        }
    };
} // namespace rw::evt
