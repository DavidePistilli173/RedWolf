#ifndef RW_DAT_VERSIONINFO_HPP
#define RW_DAT_VERSIONINFO_HPP

namespace rw::dat
{
   /**
    * @brief Aggregate for version numbers.
    */
   struct VersionInfo
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
      constexpr VersionInfo(int verMajor, int verMinor, int verPatch) : major{ verMajor }, minor{ verMinor }, patch{ verPatch } {};

      int major{ 0 }; /**< Major version number. */
      int minor{ 0 }; /**< Minor version number. */
      int patch{ 0 }; /**< Patch version number. */
   };
} // namespace rw::dat

#endif