#include "Logger.hpp"

std::unique_ptr<rw::utils::Logger> rw::utils::Logger::instance_; // Definition of the logger's instance.

namespace rw::utils
{
   Logger* Logger::instance()
   {
      if (instance_ == nullptr)
      {
         instance_.reset(new Logger());
         instance_->relInfo("Logger::instance - created logger instance.");
      }

      instance_->trace("Logger::instance - Providing logger instance.");
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
         info("Logger::setLevel - Debug log level changed from {} to {}.", level_.load(), level);
         level_ = level;
      }
   }

   void Logger::setReleaseLevel(Level level)
   {
      if constexpr (!rw::debug)
      {
         relInfo("Logger::setLevel - Release log level changed from {} to {}.", level_.load(), level);
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
   template void Logger::trace(std::string_view);
   template void Logger::trace(std::string_view, const char* const&);
   template void Logger::trace(std::string_view, const std::string_view&);
   template void Logger::trace(std::string_view, const std::string&);
   template void Logger::trace(std::string_view, const char&);
   template void Logger::trace(std::string_view, const short&);
   template void Logger::trace(std::string_view, const int&);
   template void Logger::trace(std::string_view, const long&);
   template void Logger::trace(std::string_view, const long long&);
   template void Logger::trace(std::string_view, const unsigned char&);
   template void Logger::trace(std::string_view, const unsigned short&);
   template void Logger::trace(std::string_view, const unsigned int&);
   template void Logger::trace(std::string_view, const unsigned long&);
   template void Logger::trace(std::string_view, const unsigned long long&);
   template void Logger::trace(std::string_view, const float&);
   template void Logger::trace(std::string_view, const double&);
   template void Logger::trace(std::string_view, const bool&);

   template void Logger::info(std::string_view);
   template void Logger::info(std::string_view, const char* const&);
   template void Logger::info(std::string_view, const std::string_view&);
   template void Logger::info(std::string_view, const std::string&);
   template void Logger::info(std::string_view, const char&);
   template void Logger::info(std::string_view, const short&);
   template void Logger::info(std::string_view, const int&);
   template void Logger::info(std::string_view, const long&);
   template void Logger::info(std::string_view, const long long&);
   template void Logger::info(std::string_view, const unsigned char&);
   template void Logger::info(std::string_view, const unsigned short&);
   template void Logger::info(std::string_view, const unsigned int&);
   template void Logger::info(std::string_view, const unsigned long&);
   template void Logger::info(std::string_view, const unsigned long long&);
   template void Logger::info(std::string_view, const float&);
   template void Logger::info(std::string_view, const double&);
   template void Logger::info(std::string_view, const bool&);

   template void Logger::warn(std::string_view);
   template void Logger::warn(std::string_view, const char* const&);
   template void Logger::warn(std::string_view, const std::string_view&);
   template void Logger::warn(std::string_view, const std::string&);
   template void Logger::warn(std::string_view, const char&);
   template void Logger::warn(std::string_view, const short&);
   template void Logger::warn(std::string_view, const int&);
   template void Logger::warn(std::string_view, const long&);
   template void Logger::warn(std::string_view, const long long&);
   template void Logger::warn(std::string_view, const unsigned char&);
   template void Logger::warn(std::string_view, const unsigned short&);
   template void Logger::warn(std::string_view, const unsigned int&);
   template void Logger::warn(std::string_view, const unsigned long&);
   template void Logger::warn(std::string_view, const unsigned long long&);
   template void Logger::warn(std::string_view, const float&);
   template void Logger::warn(std::string_view, const double&);
   template void Logger::warn(std::string_view, const bool&);

   template void Logger::err(std::string_view);
   template void Logger::err(std::string_view, const char* const&);
   template void Logger::err(std::string_view, const std::string_view&);
   template void Logger::err(std::string_view, const std::string&);
   template void Logger::err(std::string_view, const char&);
   template void Logger::err(std::string_view, const short&);
   template void Logger::err(std::string_view, const int&);
   template void Logger::err(std::string_view, const long&);
   template void Logger::err(std::string_view, const long long&);
   template void Logger::err(std::string_view, const unsigned char&);
   template void Logger::err(std::string_view, const unsigned short&);
   template void Logger::err(std::string_view, const unsigned int&);
   template void Logger::err(std::string_view, const unsigned long&);
   template void Logger::err(std::string_view, const unsigned long long&);
   template void Logger::err(std::string_view, const float&);
   template void Logger::err(std::string_view, const double&);
   template void Logger::err(std::string_view, const bool&);

   template void Logger::fatal(std::string_view);
   template void Logger::fatal(std::string_view, const char* const&);
   template void Logger::fatal(std::string_view, const std::string_view&);
   template void Logger::fatal(std::string_view, const std::string&);
   template void Logger::fatal(std::string_view, const char&);
   template void Logger::fatal(std::string_view, const short&);
   template void Logger::fatal(std::string_view, const int&);
   template void Logger::fatal(std::string_view, const long&);
   template void Logger::fatal(std::string_view, const long long&);
   template void Logger::fatal(std::string_view, const unsigned char&);
   template void Logger::fatal(std::string_view, const unsigned short&);
   template void Logger::fatal(std::string_view, const unsigned int&);
   template void Logger::fatal(std::string_view, const unsigned long&);
   template void Logger::fatal(std::string_view, const unsigned long long&);
   template void Logger::fatal(std::string_view, const float&);
   template void Logger::fatal(std::string_view, const double&);
   template void Logger::fatal(std::string_view, const bool&);

   template void Logger::relTrace(std::string_view);
   template void Logger::relTrace(std::string_view, const char* const&);
   template void Logger::relTrace(std::string_view, const std::string_view&);
   template void Logger::relTrace(std::string_view, const std::string&);
   template void Logger::relTrace(std::string_view, const char&);
   template void Logger::relTrace(std::string_view, const short&);
   template void Logger::relTrace(std::string_view, const int&);
   template void Logger::relTrace(std::string_view, const long&);
   template void Logger::relTrace(std::string_view, const long long&);
   template void Logger::relTrace(std::string_view, const unsigned char&);
   template void Logger::relTrace(std::string_view, const unsigned short&);
   template void Logger::relTrace(std::string_view, const unsigned int&);
   template void Logger::relTrace(std::string_view, const unsigned long&);
   template void Logger::relTrace(std::string_view, const unsigned long long&);
   template void Logger::relTrace(std::string_view, const float&);
   template void Logger::relTrace(std::string_view, const double&);
   template void Logger::relTrace(std::string_view, const bool&);

   template void Logger::relInfo(std::string_view);
   template void Logger::relInfo(std::string_view, const char* const&);
   template void Logger::relInfo(std::string_view, const std::string_view&);
   template void Logger::relInfo(std::string_view, const std::string&);
   template void Logger::relInfo(std::string_view, const char&);
   template void Logger::relInfo(std::string_view, const short&);
   template void Logger::relInfo(std::string_view, const int&);
   template void Logger::relInfo(std::string_view, const long&);
   template void Logger::relInfo(std::string_view, const long long&);
   template void Logger::relInfo(std::string_view, const unsigned char&);
   template void Logger::relInfo(std::string_view, const unsigned short&);
   template void Logger::relInfo(std::string_view, const unsigned int&);
   template void Logger::relInfo(std::string_view, const unsigned long&);
   template void Logger::relInfo(std::string_view, const unsigned long long&);
   template void Logger::relInfo(std::string_view, const float&);
   template void Logger::relInfo(std::string_view, const double&);
   template void Logger::relInfo(std::string_view, const bool&);

   template void Logger::relWarn(std::string_view);
   template void Logger::relWarn(std::string_view, const char* const&);
   template void Logger::relWarn(std::string_view, const std::string_view&);
   template void Logger::relWarn(std::string_view, const std::string&);
   template void Logger::relWarn(std::string_view, const char&);
   template void Logger::relWarn(std::string_view, const short&);
   template void Logger::relWarn(std::string_view, const int&);
   template void Logger::relWarn(std::string_view, const long&);
   template void Logger::relWarn(std::string_view, const long long&);
   template void Logger::relWarn(std::string_view, const unsigned char&);
   template void Logger::relWarn(std::string_view, const unsigned short&);
   template void Logger::relWarn(std::string_view, const unsigned int&);
   template void Logger::relWarn(std::string_view, const unsigned long&);
   template void Logger::relWarn(std::string_view, const unsigned long long&);
   template void Logger::relWarn(std::string_view, const float&);
   template void Logger::relWarn(std::string_view, const double&);
   template void Logger::relWarn(std::string_view, const bool&);

   template void Logger::relErr(std::string_view);
   template void Logger::relErr(std::string_view, const char* const&);
   template void Logger::relErr(std::string_view, const std::string_view&);
   template void Logger::relErr(std::string_view, const std::string&);
   template void Logger::relErr(std::string_view, const char&);
   template void Logger::relErr(std::string_view, const short&);
   template void Logger::relErr(std::string_view, const int&);
   template void Logger::relErr(std::string_view, const long&);
   template void Logger::relErr(std::string_view, const long long&);
   template void Logger::relErr(std::string_view, const unsigned char&);
   template void Logger::relErr(std::string_view, const unsigned short&);
   template void Logger::relErr(std::string_view, const unsigned int&);
   template void Logger::relErr(std::string_view, const unsigned long&);
   template void Logger::relErr(std::string_view, const unsigned long long&);
   template void Logger::relErr(std::string_view, const float&);
   template void Logger::relErr(std::string_view, const double&);
   template void Logger::relErr(std::string_view, const bool&);

   template void Logger::relFatal(std::string_view);
   template void Logger::relFatal(std::string_view, const char* const&);
   template void Logger::relFatal(std::string_view, const std::string_view&);
   template void Logger::relFatal(std::string_view, const std::string&);
   template void Logger::relFatal(std::string_view, const char&);
   template void Logger::relFatal(std::string_view, const short&);
   template void Logger::relFatal(std::string_view, const int&);
   template void Logger::relFatal(std::string_view, const long&);
   template void Logger::relFatal(std::string_view, const long long&);
   template void Logger::relFatal(std::string_view, const unsigned char&);
   template void Logger::relFatal(std::string_view, const unsigned short&);
   template void Logger::relFatal(std::string_view, const unsigned int&);
   template void Logger::relFatal(std::string_view, const unsigned long&);
   template void Logger::relFatal(std::string_view, const unsigned long long&);
   template void Logger::relFatal(std::string_view, const float&);
   template void Logger::relFatal(std::string_view, const double&);
   template void Logger::relFatal(std::string_view, const bool&);
} // namespace rw::utils