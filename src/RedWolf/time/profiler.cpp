//
// Created by cjm036653 on 20/09/2025.
//

#include "profiler.hpp"

#include "profiler_data.hpp"

rw::time::Profiler::Profiler(ProfilerData& data) : data_{ data }, start_{ std::chrono::high_resolution_clock::now() } {}

rw::time::Profiler::~Profiler() {
    const auto end{ std::chrono::high_resolution_clock::now() };
    const auto elapsed{ end - start_ };

    data_.elapsed_last = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    if (elapsed < data_.elapsed_min) {
        data_.elapsed_min = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    } else if (elapsed > data_.elapsed_max) {
        data_.elapsed_max = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    }
}