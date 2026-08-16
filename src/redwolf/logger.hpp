#pragma once

#include "common.hpp"
#include "profiler.hpp"

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
    concept IsFormattable = std::formattable<T, char>;

    /**
     * @brief Logging levels.
     */
    enum class LogLevel : u8 { fatal, error, warn, info, debug, trace };

    /**
     * @brief Message with a source location.
     */
    template<typename... Args>
    struct SourceMessage {
        template<typename T>
            requires(!std::same_as<std::remove_cvref_t<T>, SourceMessage>)
        consteval SourceMessage(T&& p_fmt, std::source_location p_loc = std::source_location::current()) :
            fmt{ std::forward<T>(p_fmt) }, loc{ p_loc } {}

        std::format_string<Args...> fmt;
        std::source_location        loc;
    };

    /**
     * @brief Engine logger.
     */
    class Logger {
     public:
        static constexpr usize max_message_size{ 2048 }; /**< Maximum size of a log message, with null terminator. */

        /**
         * @brief Initialise the logger.
         */
        static void init();

        /**
         * @brief Get the logger's instance.
         * @return Logger's instance.
         */
        [[nodiscard]] static Logger* instance();

        /**
         * @brief Base message logging function, should not be used directly.
         * @tparam FmtMsg Message to log, with std::format syntax.
         * @tparam Args Optional argument type for the format string.
         * @param level Logging level.
         * @param msg Message to be formatted.
         */
        template<rw::IsFormattable... Args>
        void message_base(const LogLevel level, SourceMessage<Args...> msg, Args&&... args) {
            std::array<char, max_message_size> formatted_msg{};

            // Format the header.
            const auto header_res{ std::format_to_n(
                formatted_msg.data(),
                formatted_msg.size() - 1,
                "[{:15.6f}]({}:{}) - ",
                static_cast<f64>(
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time_)
                        .count()) *
                    microseconds_to_seconds,
                msg.loc.function_name(),
                msg.loc.line()) };
            if (header_res.size > max_message_size - 1) {
                return;
            }

            const usize remaining_size{ formatted_msg.size() - header_res.size - 1 };
            const auto  final_res{ std::format_to_n(
                header_res.out, formatted_msg.size() - header_res.size - 1, msg.fmt, std::forward<Args>(args)...) };
            if (final_res.size > remaining_size) {
                return;
            }
            *final_res.out = '\0';

            switch (level) {
            case LogLevel::trace:
                std::println("\033[0;90m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::debug:
                std::println("\033[0;34m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::info:
                std::println("\033[0;32m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::warn:
                std::println("\033[0;33m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::error:
                std::println("\033[0;31m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::fatal:
                std::println("\033[0;35m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            }
        }

        /**
         * @brief Shutdown the logger.
         */
        static void shutdown();

     private:
        /**
         * @brief Default constructor.
         */
        Logger() = default;

        /**
         * @brief Starting time of the logger.
         */
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_{ std::chrono::high_resolution_clock::now() };
    };

#ifdef RW_ENABLE_LOGGING
    template<rw::IsFormattable... Args>
    void trace(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::trace, msg, std::forward<Args>(args)...);
    }

    template<rw::IsFormattable... Args>
    void debug(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::debug, msg, std::forward<Args>(args)...);
    }

    template<rw::IsFormattable... Args>
    void info(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::info, msg, std::forward<Args>(args)...);
    }

    template<rw::IsFormattable... Args>
    void warn(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::warn, msg, std::forward<Args>(args)...);
    }

    template<rw::IsFormattable... Args>
    void error(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::error, msg, std::forward<Args>(args)...);
    }

    template<rw::IsFormattable... Args>
    void fatal(SourceMessage<std::type_identity_t<Args>...> msg, Args&&... args) {
        RW_PROFILE_SCOPE
        Logger::instance()->message_base(LogLevel::fatal, msg, std::forward<Args>(args)...);
    }
#else
    template<rw::IsFormattable... Args>
    void trace([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}

    template<rw::IsFormattable... Args>
    void debug([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}

    template<rw::IsFormattable... Args>
    void info([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}

    template<rw::IsFormattable... Args>
    void warn([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}

    template<rw::IsFormattable... Args>
    void error([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}

    template<rw::IsFormattable... Args>
    void fatal([[maybe_unused]] SourceMessage<std::type_identity_t<Args>...> msg, [[maybe_unused]] Args&&... args) {}
#endif
} // namespace rw
