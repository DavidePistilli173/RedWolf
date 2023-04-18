#ifndef RW_UTIL_LOG_HPP
#define RW_UTIL_LOG_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/LogMsg.hpp"
#include "RedWolf/io/concepts.hpp"
#include "RedWolf/io/custom_formatters.hpp"

#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

namespace rw::util
{
   /**
    * @brief Custom singleton, thread-safe logger.
    * @details The default log functions only work in debug environments (RW_DEBUG macro).
    *         When using this logger, DO NOT use stdio functions, but only iostream ones (std::ios_base::sync_with_stdio(false)).
    */
   class RW_API Logger
   {
   public:
      using LogMsg = rw::dat::LogMsg;
      using Level = rw::dat::LogMsg::Level;

      /**
       * @brief Utility structure for storing a message and the source code location that generated.
       */
      struct MessageWithLocation
      {
         /**
          * @brief Constructor. It is not made explicit on purpose, so that it is transparent to the user.
          * @param pMsg Text message with ftm formatting syntax.
          * @param pLoc Source code location data.
          */
         template<typename T>
         MessageWithLocation(T pMsg, std::source_location pLoc = std::source_location::current());

         std::string_view     txt; /**< Text message with formatting syntax. */
         std::source_location loc; /**< Source code location data. */
      };

      /**
       * @brief Default constructor.
       */
      Logger();

      /**
       * @brief Log an error message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void err(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log a fatal message only in debug builds.
       * @details Throws an std::exception.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void fatal(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log an information message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void info(const MessageWithLocation& msg, const Args&... args);

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
      template<rw::io::IsFormattable... Args>
      void message_base(Level level, const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log an error message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relErr(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log a fatal message in all build types.
       * @details Throws an std::exception.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relFatal(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log an information message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relInfo(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log a trace message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relTrace(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log a warning message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relWarn(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Set the logging level for debug builds.
       * @param level New logging level for debug builds.
       */
      void setDebugLevel(Level level);

      /**
       * @brief Set the logging level for release builds.
       * @param level New logging level for release builds.
       */
      void setReleaseLevel(Level level);

      /**
       * @brief Log a trace message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void trace(const MessageWithLocation& msg, const Args&... args);

      /**
       * @brief Log a warning message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void warn(const MessageWithLocation& msg, const Args&... args);

   private:
      std::mutex         mtx_;                   /**< Logging mutex. */
      std::atomic<Level> level_{ Level::trace }; /**< Logging level. */
   };
} // namespace rw::util

// IMPLEMENTATION
template<typename T>
rw::util::Logger::MessageWithLocation::MessageWithLocation(T pMsg, std::source_location pLoc) : txt{ pMsg }, loc{ pLoc }
{
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::err(const MessageWithLocation& msg, const Args&... args)
{
   if constexpr (rw::debug) message_base(Level::error, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::fatal(const MessageWithLocation& msg, const Args&... args)
{
   if constexpr (rw::debug) message_base(Level::fatal, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::info(const MessageWithLocation& msg, const Args&... args)
{
   if constexpr (rw::debug) message_base(Level::info, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::message_base(Level level, const MessageWithLocation& msg, const Args&... args)
{
   if (level >= level_)
   {
      rw::dat::LogMsg message{ level, msg.loc, std::vformat(msg.txt, std::make_format_args(args...)) };

      std::scoped_lock lck{ mtx_ };

      switch (level)
      {
      case Level::trace:
         std::cout << "\033[0m" << std::format("{}", message) << "\033[0m\n";
         break;
      case Level::info:
         std::cout << "\033[32m" << std::format("{}", message) << "\033[0m\n";
         break;
      case Level::warning:
         std::cout << "\033[33m" << std::format("{}", message) << "\033[0m\n";
         break;
      case Level::error:
         std::cout << "\033[31m" << std::format("{}", message) << "\033[0m\n";
         break;
      case Level::fatal:
         std::cout << "\033[35m" << std::format("{}", message) << "\033[0m\n";
         throw std::exception("FATAL ERROR THROWN FROM LOGGER");
         break;
      }
   }
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::relErr(const MessageWithLocation& msg, const Args&... args)
{
   message_base(Level::error, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::relFatal(const MessageWithLocation& msg, const Args&... args)
{
   message_base(Level::fatal, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::relInfo(const MessageWithLocation& msg, const Args&... args)
{
   message_base(Level::info, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::relTrace(const MessageWithLocation& msg, const Args&... args)
{
   message_base(Level::trace, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::relWarn(const MessageWithLocation& msg, const Args&... args)
{
   message_base(Level::warning, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::trace(const MessageWithLocation& msg, const Args&... args)
{
   if constexpr (rw::debug) message_base(Level::trace, msg, args...);
}

template<rw::io::IsFormattable... Args>
void rw::util::Logger::warn(const MessageWithLocation& msg, const Args&... args)
{
   if constexpr (rw::debug) message_base(Level::warning, msg, args...);
}

#endif