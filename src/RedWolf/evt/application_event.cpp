//
// Created by cjm036653 on 12/08/25.
//

#include "application_event.hpp"

#include <string>

rw::evt::WindowResizedEvent::WindowResizedEvent(const uint32_t new_width, const uint32_t new_height) :
    Event(), width{ new_width }, height{ new_height } {}

rw::evt::EventCategory rw::evt::WindowResizedEvent::category() const {
    return compose_categories<EventCategoryType::application>();
}

std::string_view rw::evt::WindowResizedEvent::name() const {
    return "WindowResizedEvent";
}

std::string rw::evt::WindowResizedEvent::to_string() const {
    return std::string("WindowResizedEvent{ width: ") + std::to_string(width) + "; height: " + std::to_string(height) + '}';
}

rw::evt::EventType rw::evt::WindowResizedEvent::type() const {
    return EventType::window_resize;
}

rw::evt::EventCategory rw::evt::WindowCloseEvent::category() const {
    return compose_categories<EventCategoryType::application>();
}

std::string_view rw::evt::WindowCloseEvent::name() const {
    return "WindowCloseEvent";
}

rw::evt::EventType rw::evt::WindowCloseEvent::type() const {
    return rw::evt::EventType::window_close;
}