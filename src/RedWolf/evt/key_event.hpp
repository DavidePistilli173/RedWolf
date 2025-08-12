//
// Created by cjm036653 on 12/08/25.
//

#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

#include "event.hpp"

namespace rw::evt {
    /**
     * @brief Generic keyboard event.
     */
    struct KeyEvent : Event {
        /**
         * @brief Constructor.
         * @param key_code Key that was pressed or released.
         */
        explicit KeyEvent(const int32_t key_code);

        /**
         * @brief Destructor.
         */
        ~KeyEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        KeyEvent(const KeyEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        KeyEvent& operator=(const KeyEvent&) = default;

        /**
         * @brief Move constructor.
         */
        KeyEvent(KeyEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        KeyEvent& operator=(KeyEvent&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] EventCategory category() const override;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override;

        int32_t key; /**< Key code of the pressed or released key. */
    };

    /**
     * @brief Keyboard key pressed event.
     */
    struct KeyPressedEvent final : KeyEvent {
        /**
         * @brief Constructor.
         * @param key_code Key that was pressed.
         * @param repeat If true, the key was already pressed before.
         */
        explicit KeyPressedEvent(const int32_t key_code, const bool repeat = false);

        /**
         * @brief Destructor.
         */
        ~KeyPressedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        KeyPressedEvent(const KeyPressedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        KeyPressedEvent& operator=(const KeyPressedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        KeyPressedEvent(KeyPressedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        KeyPressedEvent& operator=(KeyPressedEvent&&) = default;

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

        bool is_repeat{ false }; /**< True if the key was pressed again while already being pressed. */
    };

    /**
     * @brief Keyboard key released event.
     */
    struct KeyReleasedEvent final : KeyEvent {
        /**
         * @brief Constructor.
         * @param key_code Key that was released.
         */
        explicit KeyReleasedEvent(const int32_t key_code);

        /**
         * @brief Destructor.
         */
        ~KeyReleasedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        KeyReleasedEvent(const KeyReleasedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        KeyReleasedEvent& operator=(const KeyReleasedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        KeyReleasedEvent(KeyReleasedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        KeyReleasedEvent& operator=(KeyReleasedEvent&&) = default;

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

#endif // KEY_EVENT_HPP
