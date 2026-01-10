//
// Created by cjm036653 on 20/09/2025.
//

#ifndef SRC_REDWOLF_PROFILER_DATA_HPP
#define SRC_REDWOLF_PROFILER_DATA_HPP

#include "profiler.hpp"

#include <chrono>
#include <limits>
#include <string_view>

namespace rw::time {
    class Profiler;

    /**
     * @brief Time profiler data.
     */
    struct ProfilerData {
        /**
         * @brief Constructor.
         * @param p_name Readable name of the profiler. This must remain valid for the entire lifetime of the profiler data.
         */
        explicit ProfilerData(const std::string_view p_name);

        /**
         * @brief Get the last recorded elapsed time in milliseconds.
         * @return Last recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_last_milliseconds() const;
#
        /**
         * @brief Get the minimum recorded elapsed time in milliseconds.
         * @return Minimum recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_min_milliseconds() const;

        /**
         * @brief Get the maximum recorded elapsed time in milliseconds.
         * @return Maximum recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_max_milliseconds() const;

        /**
         * @brief Start recording profiling data.
         * @return Object that computes and records profiling data.
         */
        [[nodiscard]] Profiler record();

        std::string_view name; /**< Readable name for the profiling data. */

        std::chrono::nanoseconds elapsed_min{ std::numeric_limits<int64_t>::max() }; /**< Minimum elapsed time. */
        std::chrono::nanoseconds elapsed_last{ 0 };                                  /**< Last elapsed time. */
        std::chrono::nanoseconds elapsed_max{ 0 };                                   /**< Maximum elapsed time. */
    };
} // namespace rw::time

#endif // SRC_REDWOLF_PROFILER_DATA_HPP
