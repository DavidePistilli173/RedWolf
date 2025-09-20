//
// Created by cjm036653 on 20/09/2025.
//

#ifndef SRC_REDWOLF_PROFILER_HPP
#define SRC_REDWOLF_PROFILER_HPP

#include <chrono>

namespace rw::time {
    struct ProfilerData;

    /**
     * @brief RAII time profiler.
     */
    class Profiler {
     public:
        /**
         * @brief Constructor.
         * @param data Reference to where the profiling results will be stored.
         */
        explicit Profiler(ProfilerData& data);

        /**
         * @brief Destructor, will update the profiling data and store it.
         */
        ~Profiler();

        /**
         * @brief Copy constructor.
         */
        Profiler(const Profiler&) = delete;

        /**
         * @brief Move constructor.
         */
        Profiler(Profiler&&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Profiler& operator=(const Profiler&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        Profiler& operator=(Profiler&&) = delete;

     private:
        ProfilerData& data_;

        std::chrono::time_point<std::chrono::high_resolution_clock> start_; /**< Profiling start time. */
    };
} // namespace rw::time

#endif // SRC_REDWOLF_PROFILER_HPP
