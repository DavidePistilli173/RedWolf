//
// Created by cjm036653 on 12/08/25.
//

#include "key_event.hpp"

#include <format>

rw::evt::KeyEvent::KeyEvent(const rw::input::Key key_code) : Event(), key{ key_code } {}

rw::evt::EventCategory rw::evt::KeyEvent::category() const {
    return compose_categories<EventCategoryType::input, EventCategoryType::keyboard>();
}

std::string_view rw::evt::KeyEvent::name() const {
    return "KeyEvent";
}

rw::evt::KeyPressedEvent::KeyPressedEvent(const rw::input::Key key_code, const bool repeat) : KeyEvent(key_code), is_repeat(repeat) {}

std::string_view rw::evt::KeyPressedEvent::name() const {
    return "KeyPressedEvent";
}

std::string rw::evt::KeyPressedEvent::to_string() const {
    return std::format("KeyPressedEvent{{ key: {}; repeat: {}}}", key, is_repeat);
}

rw::evt::EventType rw::evt::KeyPressedEvent::type() const {
    return EventType::key_pressed;
}

rw::evt::KeyReleasedEvent::KeyReleasedEvent(const rw::input::Key key_code) : KeyEvent(key_code) {}

std::string_view rw::evt::KeyReleasedEvent::name() const {
    return "KeyReleasedEvent";
}

std::string rw::evt::KeyReleasedEvent::to_string() const {
    return std::format("KeyReleasedEvent{{ key: {}}}", key);
}

rw::evt::EventType rw::evt::KeyReleasedEvent::type() const {
    return EventType::key_released;
}

rw::evt::KeyTypedEvent::KeyTypedEvent(const rw::input::Key key_code) : KeyEvent(key_code) {}

std::string_view rw::evt::KeyTypedEvent::name() const {
    return "KeyTypedEvent";
}

std::string rw::evt::KeyTypedEvent::to_string() const {
    return std::format("KeyTypedEvent{{ key: {}}}", key);
}

rw::evt::EventType rw::evt::KeyTypedEvent::type() const {
    return EventType::key_typed;
}