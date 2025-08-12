//
// Created by cjm036653 on 12/08/25.
//

#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include "event.hpp"

namespace rw::evt {
    /**
     * @brief Event for mouse movement.
     */
    struct MouseMovedEvent final : Event {
        /**
         * @brief Constructor.
         * @param new_x New mouse x coordinate on the screen.
         * @param new_y New mouse y coordinate on the screen.
         */
        explicit MouseMovedEvent(float new_x, float new_y);

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
        [[nodiscard]] EventCategory category() const override;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override;

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override;

        float x{ 0.0F }; /**< New mouse x coordinate. */
        float y{ 0.0F }; /**< New mouse y coordinate. */
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
        explicit MouseScrolledEvent(float p_x_offset, float p_y_offset);

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
        [[nodiscard]] EventCategory category() const override;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override;

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override;

        float x_offset{ 0.0F }; /**< Horizontal scrolling amount. */
        float y_offset{ 0.0F }; /**< Vertical scrolling amount. */
    };

    /**
     * @brief Base event for mouse buttons.
     */
    struct MouseButtonEvent : Event {
        /**
         * @brief Constructor.
         * @param button_code Code of the button that was pressed/released.
         */
        explicit MouseButtonEvent(int32_t button_code);

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
        [[nodiscard]] EventCategory category() const override;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override;

        int32_t button{ 0 }; /**< Button that was pressed or released. */
    };

    /**
     * @brief Mouse button pressed event.
     */
    struct MouseButtonPressedEvent final : MouseButtonEvent {
        /**
         * @brief Constructor.
         * @param button_code Key that was pressed.
         */
        explicit MouseButtonPressedEvent(const int32_t button_code);

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
        [[nodiscard]] std::string_view name() const override;

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override;
    };

    /**
     * @brief Mouse button released event.
     */
    struct MouseButtonReleasedEvent final : MouseButtonEvent {
        /**
         * @brief Constructor.
         * @param button_code Key that was released.
         */
        explicit MouseButtonReleasedEvent(const int32_t button_code);

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
        [[nodiscard]] std::string_view name() const override;

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override;
    };
} // namespace rw::evt

#endif // MOUSE_EVENT_HPP
