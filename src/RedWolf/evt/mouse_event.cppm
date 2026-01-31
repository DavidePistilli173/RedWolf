//
// Created by cjm036653 on 12/08/25.
//

module;

#include <format>
#include <string>
#include <string_view>

export module redwolf.evt.mouse_event;

import redwolf.evt.event;
import redwolf.input.mouse;

export namespace rw::evt {
    /**
     * @brief Event for mouse movement.
     */
    struct MouseMovedEvent final : Event {
        /**
         * @brief Constructor.
         * @param new_x New mouse x coordinate on the screen.
         * @param new_y New mouse y coordinate on the screen.
         */
        explicit MouseMovedEvent(const double new_x, const double new_y) : Event(), x{ new_x }, y{ new_y } {}

        /**
         * @brief Destructor.
         */
        ~MouseMovedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        MouseMovedEvent(const MouseMovedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        MouseMovedEvent& operator=(const MouseMovedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        MouseMovedEvent(MouseMovedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        MouseMovedEvent& operator=(MouseMovedEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<rw::evt::EventCategoryType::input, rw::evt::EventCategoryType::mouse>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "MouseMovedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("MouseMovedEvent{{ x: {}; y: {}}}", x, y);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::mouse_moved;
        }

        double x{ 0.0F }; /**< New mouse x coordinate. */
        double y{ 0.0F }; /**< New mouse y coordinate. */
    };

    /**
     * @brief Event for mouse scrolling.
     */
    struct MouseScrolledEvent final : Event {
        /**
         * @brief Constructor.
         * @param p_x_offset Horizontal scrolling amount.
         * @param p_y_offset Vertical scrolling amount.
         */
        explicit MouseScrolledEvent(double p_x_offset, double p_y_offset) : x_offset{ p_x_offset }, y_offset{ p_y_offset } {}

        /**
         * @brief Destructor.
         */
        ~MouseScrolledEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        MouseScrolledEvent(const MouseScrolledEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        MouseScrolledEvent& operator=(const MouseScrolledEvent&) = default;

        /**
         * @brief Move constructor.
         */
        MouseScrolledEvent(MouseScrolledEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        MouseScrolledEvent& operator=(MouseScrolledEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<EventCategoryType::input, rw::evt::EventCategoryType::mouse>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "MouseScrolledEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("MouseScrolledEvent{{ x_offset: {}; y_offset: {}}}", x_offset, y_offset);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::mouse_scrolled;
        }

        double x_offset{ 0.0F }; /**< Horizontal scrolling amount. */
        double y_offset{ 0.0F }; /**< Vertical scrolling amount. */
    };

    /**
     * @brief Base event for mouse buttons.
     */
    struct MouseButtonEvent : Event {
        /**
         * @brief Constructor.
         * @param button_code Code of the button that was pressed/released.
         */
        explicit MouseButtonEvent(const rw::input::MouseButton button_code) : Event(), button{ button_code } {}

        /**
         * @brief Destructor.
         */
        ~MouseButtonEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        MouseButtonEvent(const MouseButtonEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        MouseButtonEvent& operator=(const MouseButtonEvent&) = default;

        /**
         * @brief Move constructor.
         */
        MouseButtonEvent(MouseButtonEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        MouseButtonEvent& operator=(MouseButtonEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<EventCategoryType::input, EventCategoryType::mouse, EventCategoryType::mouse_button>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "MouseButtonEvent";
        }

        rw::input::MouseButton button{ rw::input::MouseButton::left }; /**< Button that was pressed or released. */
    };

    /**
     * @brief Mouse button pressed event.
     */
    struct MouseButtonPressedEvent final : MouseButtonEvent {
        /**
         * @brief Constructor.
         * @param button_code Key that was pressed.
         */
        explicit MouseButtonPressedEvent(const rw::input::MouseButton button_code) : MouseButtonEvent(button_code) {}

        /**
         * @brief Destructor.
         */
        ~MouseButtonPressedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        MouseButtonPressedEvent(const MouseButtonPressedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        MouseButtonPressedEvent& operator=(const MouseButtonPressedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        MouseButtonPressedEvent(MouseButtonPressedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        MouseButtonPressedEvent& operator=(MouseButtonPressedEvent&&) = default;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "MouseButtonPressedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("MouseButtonPressedEvent{{ button: {}}}", button);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::mouse_button_pressed;
        }
    };

    /**
     * @brief Mouse button released event.
     */
    struct MouseButtonReleasedEvent final : MouseButtonEvent {
        /**
         * @brief Constructor.
         * @param button_code Key that was released.
         */
        explicit MouseButtonReleasedEvent(const rw::input::MouseButton button_code) : MouseButtonEvent(button_code) {}

        /**
         * @brief Destructor.
         */
        ~MouseButtonReleasedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        MouseButtonReleasedEvent(const MouseButtonReleasedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        MouseButtonReleasedEvent& operator=(const MouseButtonReleasedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        MouseButtonReleasedEvent(MouseButtonReleasedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        MouseButtonReleasedEvent& operator=(MouseButtonReleasedEvent&&) = default;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "MouseButtonReleasedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("MouseButtonReleasedEvent{{ button: {}}}", button);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::mouse_button_released;
        }
    };
} // namespace rw::evt
