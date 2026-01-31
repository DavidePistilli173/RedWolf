module;

#include <chrono>

export module redwolf.time.profiler;

import redwolf.time.profiler_data;

export namespace rw::time {
    /**
     * @brief RAII time profiler.
     */
    class Profiler {
     public:
        /**
         * @brief Constructor.
         * @param data Reference to where the profiling results will be stored.
         */
        explicit Profiler(ProfilerData& data) : data_{ data }, start_{ std::chrono::high_resolution_clock::now() } {}

        /**
         * @brief Destructor, will update the profiling data and store it.
         */
        ~Profiler() {
            const auto end{ std::chrono::high_resolution_clock::now() };
            const auto elapsed{ end - start_ };

            data_.elapsed_last = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
            if (elapsed < data_.elapsed_min) {
                data_.elapsed_min = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
            } else if (elapsed > data_.elapsed_max) {
                data_.elapsed_max = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
            }
        }

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
