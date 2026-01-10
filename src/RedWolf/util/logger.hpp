//
// Created by cjm036653 on 10/08/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "../common.hpp"
#include "log_msg.hpp"

#include <atomic>
#include <memory>
#include <mutex>
#include <print>

// Macros for easy logging.
// These are some of the few macros defined in this project.
// This is done for convenience and to avoid cluttering the code with repetitive logger calls.

/**
 * @brief Log a trace message from the engine.
 * @param msg Format message, followed by the arguments.
 */
#define RW_CORE_TRACE(msg, ...) rw::util::Logger::get().trace("CORE - " msg, ##__VA_ARGS__)

/**
 * @brief Log an information message from the engine.
 * @param msg Format message, followed by the arguments.
 */
#define RW_CORE_INFO(msg, ...) rw::util::Logger::get().info("CORE - " msg, ##__VA_ARGS__)

/**
 * @brief Log a warning message from the engine.
 * @param msg Format message, followed by the arguments.
 */
#define RW_CORE_WARN(msg, ...) rw::util::Logger::get().warn("CORE - " msg, ##__VA_ARGS__)

/**
 * @brief Log an error message from the engine.
 * @param msg Format message, followed by the arguments.
 */
#define RW_CORE_ERR(msg, ...) rw::util::Logger::get().err("CORE - " msg, ##__VA_ARGS__)

/**
 * @brief Log a fatal error message from the engine.
 * @param msg Format message, followed by the arguments.
 */
#define RW_CORE_FATAL(msg, ...) rw::util::Logger::get().fatal("CORE - " msg, ##__VA_ARGS__)

/**
 * @brief Log a trace message from the user application.
 * @param msg Format message, followed by the arguments.
 */
#define RW_TRACE(msg, ...) rw::util::Logger::get().trace("APP - " msg, ##__VA_ARGS__)

/**
 * @brief Log an information message from the user application.
 * @param msg Format message, followed by the arguments.
 */
#define RW_INFO(msg, ...) rw::util::Logger::get().info("APP - " msg, ##__VA_ARGS__)

/**
 * @brief Log a warning message from the user application.
 * @param msg Format message, followed by the arguments.
 */
#define RW_WARN(msg, ...) rw::util::Logger::get().warn("APP - " msg, ##__VA_ARGS__)

/**
 * @brief Log an error message from the user application.
 * @param msg Format message, followed by the arguments.
 */
#define RW_ERR(msg, ...) rw::util::Logger::get().err("APP - " msg, ##__VA_ARGS__)

/**
 * @brief Log a fatal error message from the user application.
 * @param msg Format message, followed by the arguments.
 */
#define RW_FATAL(msg, ...) rw::util::Logger::get().fatal("APP - " msg, ##__VA_ARGS__)

namespace rw::util {
    /**
     * @brief Singleton, thread-safe logger.
     */
    class Logger {
     public:
        using Level = LogMsg::Level;

        /**
         * @brief Utility structure for storing a message and the source code location that generated.
         */
        struct MessageWithLocation {
            /**
             * @brief Constructor. It is not made explicit on purpose, so that it is transparent to the user.
             * @param p_msg Text message with ftm formatting syntax.
             * @param p_loc Source code location data.
             */
            template<typename T>
            MessageWithLocation(T p_msg, std::source_location p_loc = std::source_location::current()) : txt{ p_msg }, loc{ p_loc } {}

            std::string_view     txt; /**< Text message with formatting syntax. */
            std::source_location loc; /**< Source code location data. */
        };

        /**
         * @brief Log an error message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void err(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::error, msg, args...);
        }

        /**
         * @brief Log a fatal message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void fatal(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::fatal, msg, args...);
        }

        /**
         * @brief Get the instance of the logger.
         * @return Instance of the logger.
         */
        [[nodiscard]] static Logger& get();

        /**
         * @brief Log an information message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void info(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::info, msg, args...);
        }

        /**
         * @brief Get the current log level.
         * @return Current log level.
         */
        [[nodiscard]] Level level() const;

        /**
         * @brief Base message logging function, should not be used directly.
         * @tparam Args Optional argument type for the format string.
         * @param level Logging level.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments for the format string.
         */
        template<rw::core::IsFormattable... Args>
        void message_base(Level level, const MessageWithLocation& msg, const Args&... args) {
            if (level >= level_) {
                LogMsg message{ level, msg.loc, std::vformat(msg.txt, std::make_format_args(args...)) };

                std::scoped_lock lck{ mtx_ };

                switch (level) {
                case Level::trace:
                    std::println("\033[0m{}\033[0m", message);
                    break;
                case Level::info:
                    std::println("\033[32m{}\033[0m", message);
                    break;
                case Level::warning:
                    std::println("\033[33m{}\033[0m", message);
                    break;
                case Level::error:
                    std::println("\033[31m{}\033[0m", message);
                    break;
                case Level::fatal:
                    std::println("\033[35m{}\033[0m", message);
                    break;
                }
            }
        }

        /**
         * @brief Set the logging level for release builds.
         * @param level New logging level for release builds.
         */
        void set_level(Level level);

        /**
         * @brief Log a trace message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void trace(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::trace, msg, args...);
        }

        /**
         * @brief Log a warning message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void warn(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::warning, msg, args...);
        }

     private:
        /**
         * @brief Default constructor.
         */
        Logger();

        std::mutex         mtx_;                   /**< Logging mutex. */
        std::atomic<Level> level_{ Level::trace }; /**< Logging level. */
    };
} // namespace rw::util

#endif // LOGGER_HPP
