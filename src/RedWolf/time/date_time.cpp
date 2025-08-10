//
// Created by cjm036653 on 10/08/25.
//

#include "date_time.hpp"

std::chrono::year_month_day rw::time::DateTime::date() const {
    return date_;
}

std::chrono::day rw::time::DateTime::day() const {
    return date_.day();
}

std::chrono::hours rw::time::DateTime::hour() const {
    return time_.hours();
}

std::chrono::minutes rw::time::DateTime::minute() const {
    return time_.minutes();
}

std::chrono::month rw::time::DateTime::month() const {
    return date_.month();
}

std::chrono::seconds rw::time::DateTime::second() const {
    return time_.seconds();
}

std::chrono::hh_mm_ss<std::chrono::seconds> rw::time::DateTime::time() const {
    return time_;
}

std::chrono::year rw::time::DateTime::year() const {
    return date_.year();
}