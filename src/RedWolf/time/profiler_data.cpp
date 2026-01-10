//
// Created by cjm036653 on 20/09/2025.
//
#include "profiler_data.hpp"

#include "RedWolf/common.hpp"

rw::time::ProfilerData::ProfilerData(const std::string_view p_name) : name{ p_name } {}

float rw::time::ProfilerData::elapsed_last_milliseconds() const {
    return static_cast<float>(elapsed_last.count()) * nanoseconds_to_milliseconds;
}

float rw::time::ProfilerData::elapsed_min_milliseconds() const {
    return static_cast<float>(elapsed_min.count()) * nanoseconds_to_milliseconds;
}

float rw::time::ProfilerData::elapsed_max_milliseconds() const {
    return static_cast<float>(elapsed_max.count()) * nanoseconds_to_milliseconds;
}

rw::time::Profiler rw::time::ProfilerData::record() {
    return Profiler(*this);
}