#include "DateTime.hpp"

namespace rw::time
{
   std::chrono::year_month_day DateTime::date() const
   {
      return date_;
   }

   std::chrono::day DateTime::day() const
   {
      return date_.day();
   }

   std::chrono::hours DateTime::hour() const
   {
      return time_.hours();
   }

   std::chrono::minutes DateTime::minute() const
   {
      return time_.minutes();
   }

   std::chrono::month DateTime::month() const
   {
      return date_.month();
   }

   std::chrono::seconds DateTime::second() const
   {
      return time_.seconds();
   }

   std::chrono::hh_mm_ss<std::chrono::seconds> DateTime::time() const
   {
      return time_;
   }

   std::chrono::year DateTime::year() const
   {
      return date_.year();
   }
} // namespace rw::time