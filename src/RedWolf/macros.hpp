//
// Created by david on 17/01/2026.
//

#ifndef SRC_REDWOLF_MACROS_HPP
#define SRC_REDWOLF_MACROS_HPP

// To use these macros, you need to first include this file, then import the following modules:
// import redwolf.util.logger;

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

#endif // SRC_REDWOLF_MACROS_HPP
