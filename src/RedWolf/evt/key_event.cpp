//
// Created by cjm036653 on 12/08/25.
//

#include "key_event.hpp"

rw::evt::KeyEvent::KeyEvent(const int32_t key_code) : Event(), key{ key_code } {}

rw::evt::EventCategory rw::evt::KeyEvent::category() const {
    return compose_categories<EventCategoryType::input, EventCategoryType::keyboard>();
}

std::string_view rw::evt::KeyEvent::name() const {
    return "KeyEvent";
}

rw::evt::KeyPressedEvent::KeyPressedEvent(const int32_t key_code, const bool repeat) : KeyEvent(key_code), is_repeat(repeat) {}

std::string_view rw::evt::KeyPressedEvent::name() const {
    return "KeyPressedEvent";
}

std::string rw::evt::KeyPressedEvent::to_string() const {
    return std::string("KeyPressedEvent{ key: ") + std::to_string(key) + "; repeat: " + std::to_string(is_repeat) + '}';
}

rw::evt::EventType rw::evt::KeyPressedEvent::type() const {
    return EventType::key_pressed;
}

rw::evt::KeyReleasedEvent::KeyReleasedEvent(const int32_t key_code) : KeyEvent(key_code) {}

std::string_view rw::evt::KeyReleasedEvent::name() const {
    return "KeyReleasedEvent";
}

std::string rw::evt::KeyReleasedEvent::to_string() const {
    return std::string("KeyReleasedEvent{ key: ") + std::to_string(key) + '}';
}

rw::evt::EventType rw::evt::KeyReleasedEvent::type() const {
    return EventType::key_released;
}