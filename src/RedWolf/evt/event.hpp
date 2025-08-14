//
// Created by cjm036653 on 11/08/25.
//

#ifndef EVENT_HPP
#define EVENT_HPP

#include <concepts>
#include <cstdint>
#include <format>
#include <string_view>
#include <type_traits>

namespace rw::evt {
    /**
     * @brief Available event types within the engine.
     */
    enum class EventType : uint8_t {
        none,
        window_close,
        window_resize,
        window_focus,
        window_lost_focus,
        window_moved,
        app_tick,
        app_update,
        app_render,
        key_pressed,
        key_released,
        mouse_button_pressed,
        mouse_button_released,
        mouse_moved,
        mouse_scrolled
    };

    /**
     * @brief Categories that an event can belong to.
     */
    enum class EventCategoryType : uint32_t {
        none         = 0b00000000000000000000000000000000,
        application  = 0b00000000000000000000000000000001,
        input        = 0b00000000000000000000000000000010,
        keyboard     = 0b00000000000000000000000000000100,
        mouse        = 0b00000000000000000000000000001000,
        mouse_button = 0b00000000000000000000000000010000
    };

    /**
     * @brief Type used to store the categories of an event.
     */
    using EventCategory = std::underlying_type_t<EventCategoryType>;

    /**
     * @brief Compose multiple event categories into a single category mask.
     * @tparam Categories List of event categories to compose.
     */
    template<EventCategoryType... Categories>
    [[nodiscard]] consteval EventCategory compose_categories() {
        return (static_cast<EventCategory>(Categories) | ...);
    }

    /**
     * @brief Base event class.
     */
    struct Event {
        /**
         * @brief Constructor.
         */
        Event() = default;

        /**
         * @brief Destructor.
         */
        virtual ~Event() = default;

        /**
         * @brief Copy constructor.
         */
        Event(const Event&) = default;

        /**
         * @brief Copy-assignment operator.
         */
        Event& operator=(const Event&) = default;

        /**
         * @brief Move constructor.
         */
        Event(Event&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        Event& operator=(Event&&) = default;

        /**
         * @brief Get the category flags of the event.
         */
        [[nodiscard]] virtual EventCategory category() const = 0;

        /**
         * @brief Check if the event belongs to a given category mask.
         * @param category_to_check Category mask to check.
         * @return True if the event belongs to at least one of the specified categories. False otherwise.
         */
        [[nodiscard]] bool is_in_category(const EventCategory category_to_check) const;

        /**
         * @brief Get the readable name of the event.
         */
        [[nodiscard]] virtual std::string_view name() const = 0;

        /**
         * @brief Get a string representation of the event.
         * @return String representation of the event.
         */
        [[nodiscard]] virtual std::string to_string() const;

        /**
         * @brief Get the type of event.
         */
        [[nodiscard]] virtual EventType type() const = 0;
    };
} // namespace rw::evt

/**
 * @brief std::formatter specialization for rw::evt::Event
 */
template<std::derived_from<rw::evt::Event> T>
struct std::formatter<T> : std::formatter<std::string> {
    auto format(const rw::evt::Event& event, std::format_context& ctx) const {
        return std::formatter<std::string>::format(event.to_string(), ctx);
    }
};
#endif // EVENT_HPP
