//
// Created by cjm036653 on 12/08/25.
//

#ifndef APPLICATION_EVENT_HPP
#define APPLICATION_EVENT_HPP

#include "event.hpp"

#include <cstdint>

namespace rw::evt {
    /**
     * @brief Window resizing event.
     */
    struct WindowResizeEvent final : Event {
        /**
         * @brief Constructor.
         * @param new_width New window width.
         * @param new_height New window height.
         */
        WindowResizeEvent(const uint32_t new_width, const uint32_t new_height);

        /**
         * @brief Destructor.
         */
        ~WindowResizeEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        WindowResizeEvent(const WindowResizeEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        WindowResizeEvent& operator=(const WindowResizeEvent&) = default;

        /**
         * @brief Move constructor.
         */
        WindowResizeEvent(WindowResizeEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        WindowResizeEvent& operator=(WindowResizeEvent&&) = default;

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

        uint32_t width{ 0 };  /**< New window width. */
        uint32_t height{ 0 }; /**< New window height. */
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
        [[nodiscard]] EventCategory category() const override;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override;
    };
} // namespace rw::evt

#endif // APPLICATION_EVENT_HPP
