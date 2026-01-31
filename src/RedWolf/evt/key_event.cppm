module;

#include <format>
#include <string>
#include <string_view>

export module redwolf.evt.key_event;

import redwolf.evt.event;
import redwolf.input.keyboard;

export namespace rw::evt {
    /**
     * @brief Generic keyboard event.
     */
    struct KeyEvent : Event {
        /**
         * @brief Constructor.
         * @param key_code Key that was pressed or released.
         */
        explicit KeyEvent(const rw::input::Key key_code) : Event(), key{ key_code } {}

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
        [[nodiscard]] EventCategory category() const override {
            return compose_categories<EventCategoryType::input, EventCategoryType::keyboard>();
        }

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "KeyEvent";
        }

        rw::input::Key key{ rw::input::Key::a }; /**< Key code of the pressed or released key. */
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
        explicit KeyPressedEvent(const rw::input::Key key_code, const bool repeat = false) : KeyEvent(key_code), is_repeat(repeat) {}

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
        [[nodiscard]] std::string_view name() const override {
            return "KeyPressedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("KeyPressedEvent{{ key: {}; repeat: {}}}", key, is_repeat);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::key_pressed;
        }

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
        explicit KeyReleasedEvent(const rw::input::Key key_code) : KeyEvent(key_code) {}

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
        [[nodiscard]] std::string_view name() const override {
            return "KeyReleasedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("KeyReleasedEvent{{ key: {}}}", key);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::key_released;
        }
    };

    /**
     * @brief Keyboard key typed event.
     */
    struct KeyTypedEvent final : KeyEvent {
        /**
         * @brief Constructor.
         * @param key_code Key that was pressed.
         */
        explicit KeyTypedEvent(const rw::input::Key key_code) : KeyEvent(key_code) {}

        /**
         * @brief Destructor.
         */
        ~KeyTypedEvent() override = default;

        /**
         * @brief Copy constructor.
         */
        KeyTypedEvent(const KeyTypedEvent&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        KeyTypedEvent& operator=(const KeyTypedEvent&) = default;

        /**
         * @brief Move constructor.
         */
        KeyTypedEvent(KeyTypedEvent&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        KeyTypedEvent& operator=(KeyTypedEvent&&) = default;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] std::string_view name() const override {
            return "KeyTypedEvent";
        }

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] std::string to_string() const override {
            return std::format("KeyTypedEvent{{ key: {}}}", key);
        }

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] EventType type() const override {
            return EventType::key_typed;
        }
    };
} // namespace rw::evt
