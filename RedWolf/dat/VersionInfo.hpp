#ifndef RW_DAT_VERSIONINFO_HPP
#define RW_DAT_VERSIONINFO_HPP

#include "RedWolf/common.hpp"

namespace rw::dat
{
   /**
    * @brief Aggregate for version numbers.
    */
   struct RW_API VersionInfo
   {
      /**
       * @brief Default constructor.
       */
      constexpr VersionInfo() = default;

      /**
       * @brief Construct a custom struct.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       */
      constexpr VersionInfo(u8 verMajor, u8 verMinor, u8 verPatch) : major{ verMajor }, minor{ verMinor }, patch{ verPatch } {};

      u8 major{ 0U }; /**< Major version number. */
      u8 minor{ 0U }; /**< Minor version number. */
      u8 patch{ 0U }; /**< Patch version number. */
   };
} // namespace rw::dat

#endif