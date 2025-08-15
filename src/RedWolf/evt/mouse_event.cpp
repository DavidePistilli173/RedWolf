//
// Created by cjm036653 on 12/08/25.
//

#include "mouse_event.hpp"

#include <format>

rw::evt::MouseMovedEvent::MouseMovedEvent(const double new_x, const double new_y) : Event(), x{ new_x }, y{ new_y } {}

rw::evt::EventCategory rw::evt::MouseMovedEvent::category() const {
    return compose_categories<rw::evt::EventCategoryType::input, rw::evt::EventCategoryType::mouse>();
}

std::string_view rw::evt::MouseMovedEvent::name() const {
    return "MouseMovedEvent";
}

std::string rw::evt::MouseMovedEvent::to_string() const {
    return std::format("MouseMovedEvent{{ x: {}; y: {}}}", x, y);
}

rw::evt::EventType rw::evt::MouseMovedEvent::type() const {
    return EventType::mouse_moved;
}

rw::evt::MouseScrolledEvent::MouseScrolledEvent(double p_x_offset, double p_y_offset) : x_offset{ p_x_offset }, y_offset{ p_y_offset } {}

rw::evt::EventCategory rw::evt::MouseScrolledEvent::category() const {
    return compose_categories<EventCategoryType::input, rw::evt::EventCategoryType::mouse>();
}

std::string_view rw::evt::MouseScrolledEvent::name() const {
    return "MouseScrolledEvent";
}

std::string rw::evt::MouseScrolledEvent::to_string() const {
    return std::format("MouseScrolledEvent{{ x_offset: {}; y_offset: {}}}", x_offset, y_offset);
}

rw::evt::EventType rw::evt::MouseScrolledEvent::type() const {
    return EventType::mouse_scrolled;
}

rw::evt::MouseButtonEvent::MouseButtonEvent(const rw::input::MouseButton button_code) : Event(), button{ button_code } {}

rw::evt::EventCategory rw::evt::MouseButtonEvent::category() const {
    return compose_categories<EventCategoryType::input, EventCategoryType::mouse, EventCategoryType::mouse_button>();
}

std::string_view rw::evt::MouseButtonEvent::name() const {
    return "MouseButtonEvent";
}

rw::evt::MouseButtonPressedEvent::MouseButtonPressedEvent(const rw::input::MouseButton button_code) : MouseButtonEvent(button_code) {}

std::string_view rw::evt::MouseButtonPressedEvent::name() const {
    return "MouseButtonPressedEvent";
}

std::string rw::evt::MouseButtonPressedEvent::to_string() const {
    return std::format("MouseButtonPressedEvent{{ button: {}}}", button);
}

rw::evt::EventType rw::evt::MouseButtonPressedEvent::type() const {
    return EventType::mouse_button_pressed;
}

rw::evt::MouseButtonReleasedEvent::MouseButtonReleasedEvent(const rw::input::MouseButton button_code) : MouseButtonEvent(button_code) {}

std::string_view rw::evt::MouseButtonReleasedEvent::name() const {
    return "MouseButtonReleasedEvent";
}

std::string rw::evt::MouseButtonReleasedEvent::to_string() const {
    return std::format("MouseButtonReleasedEvent{{ button: {}}}", button);
}

rw::evt::EventType rw::evt::MouseButtonReleasedEvent::type() const {
    return EventType::mouse_button_released;
}
