//
// Created by cjm036653 on 11/08/25.
//

#include "event.hpp"

bool rw::evt::Event::is_in_category(const EventCategory category_to_check) const {
    return category() & category_to_check;
}

std::string rw::evt::Event::to_string() const {
    return std::string(name());
}