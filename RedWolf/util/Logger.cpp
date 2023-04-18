#include "Logger.hpp"

using namespace rw::util;

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

   relInfo("Logger created.");
};

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