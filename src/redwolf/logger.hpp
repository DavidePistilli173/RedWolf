#pragma once

#include "redwolf/common.hpp"

#include <array>
#include <chrono>
#include <format>
#include <print>
#include <source_location>

namespace rw {
    /**
     * @brief Concept for objects that can be used as std::format arguments.
     */
    template<typename T>
    concept IsFormattable = std::is_default_constructible_v<std::formatter<T>>;

    /**
     * @brief Logging levels.
     */
    enum class LogLevel : u8 { fatal, error, warn, info, trace };

    /**
     * @brief Engine logger.
     */
    class Logger {
     public:
        static constexpr usize max_message_size{ 2048 }; /**< Maximum size of a log message, with null terminator. */

        template<usize N>
        struct FixedString {
            std::array<char, N> data{};

            consteval FixedString(const std::array<char, N>& str) : data{ str } {}

            consteval operator std::string_view() const {
                return { data.data(), N - 1 };
            }
        };

        /**
         * @brief Utility structure for storing a message and the source code location that generated it.
         */
        struct MessageWithLocation {
            /**
             * @brief Constructor. It is not made explicit on purpose, so that it is transparent to the user.
             * @param p_msg Text message with ftm formatting syntax.
             * @param p_loc Source code location data.
             */
            template<typename T>
            MessageWithLocation(T p_msg, const std::source_location p_loc = std::source_location::current()) : txt{ p_msg }, loc{ p_loc } {}

            std::string_view     txt; /**< Text message with formatting syntax. */
            std::source_location loc; /**< Source code location data. */
        };

        /**
         * @brief Log an error message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::IsFormattable... Args>
        void err(const MessageWithLocation& msg, const Args&... args) {
            message_base(LogLevel::error, msg, args...);
        }

        /**
         * @brief Log a fatal message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::IsFormattable... Args>
        void fatal(const MessageWithLocation& msg, const Args&... args) {
            message_base(LogLevel::fatal, msg, args...);
        }

        /**
         * @brief Log an information message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::IsFormattable... Args>
        void info(const MessageWithLocation& msg, const Args&... args) {
            message_base(LogLevel::info, msg, args...);
        }

        /**
         * @brief Base message logging function, should not be used directly.
         * @tparam Args Optional argument type for the format string.
         * @param level Logging level.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments for the format string.
         */
        template<FixedString FmtMsg, rw::IsFormattable... Args>
        void message_base(const LogLevel level, const MessageWithLocation& msg, Args&&... args) {
            std::array<char, max_message_size> formatted_msg{};

            // Format the header.
            const auto header_res{ std::format_to_n(
                formatted_msg.data(),
                formatted_msg.size() - 1,
                "[{:30.6}] - ",
                static_cast<f64>(
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time_)
                        .count()) *
                    microseconds_to_seconds) };
            if (header_res.size > max_message_size - 1) {
                return;
            }

            const usize remaining_size{ formatted_msg.size() - header_res.size - 1 };
            const auto  final_res{ std::format_to_n(
                header_res.out, formatted_msg.size() - header_res.size - 1, FmtMsg, std::forward<Args>(args)...) };
            if (final_res.size > remaining_size) {
                return;
            }
            *final_res.out = '\0';

            switch (level) {
            case LogLevel::trace:
                std::println("\033[0m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::info:
                std::println("\033[32m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::warn:
                std::println("\033[33m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::error:
                std::println("\033[31m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::fatal:
                std::println("\033[35m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            }
        }

        /**
         * @brief Log a trace message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::IsFormattable... Args>
        void trace(const MessageWithLocation& msg, const Args&... args) {
            message_base(LogLevel::trace, msg, args...);
        }

        /**
         * @brief Log a warning message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::IsFormattable... Args>
        void warn(const MessageWithLocation& msg, const Args&... args) {
            message_base(LogLevel::warn, msg, args...);
        }

     private:
        /**
         * @brief Default constructor.
         */
        Logger() {
            info("Logger created.");
        }

        /**
         * @brief Starting time of the logger.
         */
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_{ std::chrono::high_resolution_clock::now() };
    };
} // namespace rw