#ifndef RW_DAT_LOGMSG_HPP
#define RW_DAT_LOGMSG_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/time/DateTime.hpp"

#include <filesystem>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

namespace rw::dat
{
   /**
    * @brief Struct containing a log message.
    */
   struct RW_API LogMsg
   {
      /**
       * @brief Logging levels.
       */
      enum class Level
      {
         trace = 0,
         info = 1,
         warning = 2,
         error = 3,
         fatal = 4
      };

      /**
       * @brief Construct a new log message with the current date and time.
       * @param lvl Logging level of the message.
       * @param srcLoc Location in the source code that generated the message.
       * @param msg Actual message.
       */
      LogMsg(Level lvl, std::source_location srcLoc, std::string_view msg);

      Level                level;          /**< Level of the message. */
      rw::time::DateTime   dateTime;       /**< Date and time of the message. */
      std::source_location sourceLocation; /**< Info about the source code location where the message was generated. */
      std::string          message;        /**< Actual message. */
   };
} // namespace rw::dat

/**
 * @brief std::formatter specialization for rw::utils::LogMsg::Level
 */
template<>
struct std::formatter<rw::dat::LogMsg::Level> : std::formatter<std::string>
{
   auto format(rw::dat::LogMsg::Level lvl, std::format_context& ctx)
   {
      switch (lvl)
      {
      case rw::dat::LogMsg::Level::trace:
         return std::formatter<std::string>::format(std::format("{}", "TRACE"), ctx);
         break;
      case rw::dat::LogMsg::Level::info:
         return std::formatter<std::string>::format(std::format("{}", "INFO"), ctx);
         break;
      case rw::dat::LogMsg::Level::warning:
         return std::formatter<std::string>::format(std::format("{}", "WARNING"), ctx);
         break;
      case rw::dat::LogMsg::Level::error:
         return std::formatter<std::string>::format(std::format("{}", "ERROR"), ctx);
         break;
      case rw::dat::LogMsg::Level::fatal:
         return std::formatter<std::string>::format(std::format("{}", "FATAL"), ctx);
         break;
      }
      return std::formatter<std::string>::format(std::format("{}", "INVALID"), ctx);
   }
};

/**
 * @brief std::formatter specialization for rw::dat::LogMsg
 */
template<>
struct std::formatter<rw::dat::LogMsg> : std::formatter<std::string>
{
   auto format(rw::dat::LogMsg msg, std::format_context& ctx)
   {
      std::filesystem::path filePath{ msg.sourceLocation.file_name() };

      return std::formatter<std::string>::format(
         std::format(
            "[{}] - <{}> ({}::{}({})) {}",
            msg.level,
            msg.dateTime,
            filePath.filename().string(),
            msg.sourceLocation.function_name(),
            msg.sourceLocation.line(),
            msg.message),
         ctx);
   }
};

#endif