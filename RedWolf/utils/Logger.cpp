#include "Logger.hpp"

std::unique_ptr<rw::utils::Logger> rw::utils::Logger::instance_; // Definition of the logger's instance.

namespace rw::utils
{
   Logger* Logger::instance()
   {
      if (instance_ == nullptr)
      {
         instance_.reset(new Logger());
         instance_->relInfo("Created logger instance.");
      }

      instance_->trace("Providing logger instance.");
      return instance_.get();
   }

   Logger::Level Logger::level() const
   {
      return level_;
   }

   void Logger::setDebugLevel(Level level)
   {
      if constexpr (rw::debug)
      {
         info("Debug log level changed from {} to {}.", level_.load(), level);
         level_ = level;
      }
   }

   void Logger::setReleaseLevel(Level level)
   {
      if constexpr (!rw::debug)
      {
         relInfo("Release log level changed from {} to {}.", level_.load(), level);
         level_ = level;
      }
   }

   Logger::Logger()
   {
      std::ios_base::sync_with_stdio(false); // Stop syncing stdio and iostream, for speed.

      // Default log levels for debug and release builds.
      if constexpr (rw::debug)
      {
         level_ = Level::trace;
      }
      else
      {
         level_ = Level::warning;
      }
   };

   // Explicit template instantiation for common types.
   template void Logger::trace(const MessageWithLocation&);
   template void Logger::trace(const MessageWithLocation&, const char* const&);
   template void Logger::trace(const MessageWithLocation&, const std::string_view&);
   template void Logger::trace(const MessageWithLocation&, const std::string&);
   template void Logger::trace(const MessageWithLocation&, const char&);
   template void Logger::trace(const MessageWithLocation&, const short&);
   template void Logger::trace(const MessageWithLocation&, const int&);
   template void Logger::trace(const MessageWithLocation&, const long&);
   template void Logger::trace(const MessageWithLocation&, const long long&);
   template void Logger::trace(const MessageWithLocation&, const unsigned char&);
   template void Logger::trace(const MessageWithLocation&, const unsigned short&);
   template void Logger::trace(const MessageWithLocation&, const unsigned int&);
   template void Logger::trace(const MessageWithLocation&, const unsigned long&);
   template void Logger::trace(const MessageWithLocation&, const unsigned long long&);
   template void Logger::trace(const MessageWithLocation&, const float&);
   template void Logger::trace(const MessageWithLocation&, const double&);
   template void Logger::trace(const MessageWithLocation&, const bool&);

   template void Logger::info(const MessageWithLocation&);
   template void Logger::info(const MessageWithLocation&, const char* const&);
   template void Logger::info(const MessageWithLocation&, const std::string_view&);
   template void Logger::info(const MessageWithLocation&, const std::string&);
   template void Logger::info(const MessageWithLocation&, const char&);
   template void Logger::info(const MessageWithLocation&, const short&);
   template void Logger::info(const MessageWithLocation&, const int&);
   template void Logger::info(const MessageWithLocation&, const long&);
   template void Logger::info(const MessageWithLocation&, const long long&);
   template void Logger::info(const MessageWithLocation&, const unsigned char&);
   template void Logger::info(const MessageWithLocation&, const unsigned short&);
   template void Logger::info(const MessageWithLocation&, const unsigned int&);
   template void Logger::info(const MessageWithLocation&, const unsigned long&);
   template void Logger::info(const MessageWithLocation&, const unsigned long long&);
   template void Logger::info(const MessageWithLocation&, const float&);
   template void Logger::info(const MessageWithLocation&, const double&);
   template void Logger::info(const MessageWithLocation&, const bool&);

   template void Logger::warn(const MessageWithLocation&);
   template void Logger::warn(const MessageWithLocation&, const char* const&);
   template void Logger::warn(const MessageWithLocation&, const std::string_view&);
   template void Logger::warn(const MessageWithLocation&, const std::string&);
   template void Logger::warn(const MessageWithLocation&, const char&);
   template void Logger::warn(const MessageWithLocation&, const short&);
   template void Logger::warn(const MessageWithLocation&, const int&);
   template void Logger::warn(const MessageWithLocation&, const long&);
   template void Logger::warn(const MessageWithLocation&, const long long&);
   template void Logger::warn(const MessageWithLocation&, const unsigned char&);
   template void Logger::warn(const MessageWithLocation&, const unsigned short&);
   template void Logger::warn(const MessageWithLocation&, const unsigned int&);
   template void Logger::warn(const MessageWithLocation&, const unsigned long&);
   template void Logger::warn(const MessageWithLocation&, const unsigned long long&);
   template void Logger::warn(const MessageWithLocation&, const float&);
   template void Logger::warn(const MessageWithLocation&, const double&);
   template void Logger::warn(const MessageWithLocation&, const bool&);

   template void Logger::err(const MessageWithLocation&);
   template void Logger::err(const MessageWithLocation&, const char* const&);
   template void Logger::err(const MessageWithLocation&, const std::string_view&);
   template void Logger::err(const MessageWithLocation&, const std::string&);
   template void Logger::err(const MessageWithLocation&, const char&);
   template void Logger::err(const MessageWithLocation&, const short&);
   template void Logger::err(const MessageWithLocation&, const int&);
   template void Logger::err(const MessageWithLocation&, const long&);
   template void Logger::err(const MessageWithLocation&, const long long&);
   template void Logger::err(const MessageWithLocation&, const unsigned char&);
   template void Logger::err(const MessageWithLocation&, const unsigned short&);
   template void Logger::err(const MessageWithLocation&, const unsigned int&);
   template void Logger::err(const MessageWithLocation&, const unsigned long&);
   template void Logger::err(const MessageWithLocation&, const unsigned long long&);
   template void Logger::err(const MessageWithLocation&, const float&);
   template void Logger::err(const MessageWithLocation&, const double&);
   template void Logger::err(const MessageWithLocation&, const bool&);

   template void Logger::fatal(const MessageWithLocation&);
   template void Logger::fatal(const MessageWithLocation&, const char* const&);
   template void Logger::fatal(const MessageWithLocation&, const std::string_view&);
   template void Logger::fatal(const MessageWithLocation&, const std::string&);
   template void Logger::fatal(const MessageWithLocation&, const char&);
   template void Logger::fatal(const MessageWithLocation&, const short&);
   template void Logger::fatal(const MessageWithLocation&, const int&);
   template void Logger::fatal(const MessageWithLocation&, const long&);
   template void Logger::fatal(const MessageWithLocation&, const long long&);
   template void Logger::fatal(const MessageWithLocation&, const unsigned char&);
   template void Logger::fatal(const MessageWithLocation&, const unsigned short&);
   template void Logger::fatal(const MessageWithLocation&, const unsigned int&);
   template void Logger::fatal(const MessageWithLocation&, const unsigned long&);
   template void Logger::fatal(const MessageWithLocation&, const unsigned long long&);
   template void Logger::fatal(const MessageWithLocation&, const float&);
   template void Logger::fatal(const MessageWithLocation&, const double&);
   template void Logger::fatal(const MessageWithLocation&, const bool&);

   template void Logger::relTrace(const MessageWithLocation&);
   template void Logger::relTrace(const MessageWithLocation&, const char* const&);
   template void Logger::relTrace(const MessageWithLocation&, const std::string_view&);
   template void Logger::relTrace(const MessageWithLocation&, const std::string&);
   template void Logger::relTrace(const MessageWithLocation&, const char&);
   template void Logger::relTrace(const MessageWithLocation&, const short&);
   template void Logger::relTrace(const MessageWithLocation&, const int&);
   template void Logger::relTrace(const MessageWithLocation&, const long&);
   template void Logger::relTrace(const MessageWithLocation&, const long long&);
   template void Logger::relTrace(const MessageWithLocation&, const unsigned char&);
   template void Logger::relTrace(const MessageWithLocation&, const unsigned short&);
   template void Logger::relTrace(const MessageWithLocation&, const unsigned int&);
   template void Logger::relTrace(const MessageWithLocation&, const unsigned long&);
   template void Logger::relTrace(const MessageWithLocation&, const unsigned long long&);
   template void Logger::relTrace(const MessageWithLocation&, const float&);
   template void Logger::relTrace(const MessageWithLocation&, const double&);
   template void Logger::relTrace(const MessageWithLocation&, const bool&);

   template void Logger::relInfo(const MessageWithLocation&);
   template void Logger::relInfo(const MessageWithLocation&, const char* const&);
   template void Logger::relInfo(const MessageWithLocation&, const std::string_view&);
   template void Logger::relInfo(const MessageWithLocation&, const std::string&);
   template void Logger::relInfo(const MessageWithLocation&, const char&);
   template void Logger::relInfo(const MessageWithLocation&, const short&);
   template void Logger::relInfo(const MessageWithLocation&, const int&);
   template void Logger::relInfo(const MessageWithLocation&, const long&);
   template void Logger::relInfo(const MessageWithLocation&, const long long&);
   template void Logger::relInfo(const MessageWithLocation&, const unsigned char&);
   template void Logger::relInfo(const MessageWithLocation&, const unsigned short&);
   template void Logger::relInfo(const MessageWithLocation&, const unsigned int&);
   template void Logger::relInfo(const MessageWithLocation&, const unsigned long&);
   template void Logger::relInfo(const MessageWithLocation&, const unsigned long long&);
   template void Logger::relInfo(const MessageWithLocation&, const float&);
   template void Logger::relInfo(const MessageWithLocation&, const double&);
   template void Logger::relInfo(const MessageWithLocation&, const bool&);

   template void Logger::relWarn(const MessageWithLocation&);
   template void Logger::relWarn(const MessageWithLocation&, const char* const&);
   template void Logger::relWarn(const MessageWithLocation&, const std::string_view&);
   template void Logger::relWarn(const MessageWithLocation&, const std::string&);
   template void Logger::relWarn(const MessageWithLocation&, const char&);
   template void Logger::relWarn(const MessageWithLocation&, const short&);
   template void Logger::relWarn(const MessageWithLocation&, const int&);
   template void Logger::relWarn(const MessageWithLocation&, const long&);
   template void Logger::relWarn(const MessageWithLocation&, const long long&);
   template void Logger::relWarn(const MessageWithLocation&, const unsigned char&);
   template void Logger::relWarn(const MessageWithLocation&, const unsigned short&);
   template void Logger::relWarn(const MessageWithLocation&, const unsigned int&);
   template void Logger::relWarn(const MessageWithLocation&, const unsigned long&);
   template void Logger::relWarn(const MessageWithLocation&, const unsigned long long&);
   template void Logger::relWarn(const MessageWithLocation&, const float&);
   template void Logger::relWarn(const MessageWithLocation&, const double&);
   template void Logger::relWarn(const MessageWithLocation&, const bool&);

   template void Logger::relErr(const MessageWithLocation&);
   template void Logger::relErr(const MessageWithLocation&, const char* const&);
   template void Logger::relErr(const MessageWithLocation&, const std::string_view&);
   template void Logger::relErr(const MessageWithLocation&, const std::string&);
   template void Logger::relErr(const MessageWithLocation&, const char&);
   template void Logger::relErr(const MessageWithLocation&, const short&);
   template void Logger::relErr(const MessageWithLocation&, const int&);
   template void Logger::relErr(const MessageWithLocation&, const long&);
   template void Logger::relErr(const MessageWithLocation&, const long long&);
   template void Logger::relErr(const MessageWithLocation&, const unsigned char&);
   template void Logger::relErr(const MessageWithLocation&, const unsigned short&);
   template void Logger::relErr(const MessageWithLocation&, const unsigned int&);
   template void Logger::relErr(const MessageWithLocation&, const unsigned long&);
   template void Logger::relErr(const MessageWithLocation&, const unsigned long long&);
   template void Logger::relErr(const MessageWithLocation&, const float&);
   template void Logger::relErr(const MessageWithLocation&, const double&);
   template void Logger::relErr(const MessageWithLocation&, const bool&);

   template void Logger::relFatal(const MessageWithLocation&);
   template void Logger::relFatal(const MessageWithLocation&, const char* const&);
   template void Logger::relFatal(const MessageWithLocation&, const std::string_view&);
   template void Logger::relFatal(const MessageWithLocation&, const std::string&);
   template void Logger::relFatal(const MessageWithLocation&, const char&);
   template void Logger::relFatal(const MessageWithLocation&, const short&);
   template void Logger::relFatal(const MessageWithLocation&, const int&);
   template void Logger::relFatal(const MessageWithLocation&, const long&);
   template void Logger::relFatal(const MessageWithLocation&, const long long&);
   template void Logger::relFatal(const MessageWithLocation&, const unsigned char&);
   template void Logger::relFatal(const MessageWithLocation&, const unsigned short&);
   template void Logger::relFatal(const MessageWithLocation&, const unsigned int&);
   template void Logger::relFatal(const MessageWithLocation&, const unsigned long&);
   template void Logger::relFatal(const MessageWithLocation&, const unsigned long long&);
   template void Logger::relFatal(const MessageWithLocation&, const float&);
   template void Logger::relFatal(const MessageWithLocation&, const double&);
   template void Logger::relFatal(const MessageWithLocation&, const bool&);
} // namespace rw::utils