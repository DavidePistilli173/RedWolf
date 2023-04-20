#include "VersionInfo.hpp"

using namespace rw::dat;

constexpr VersionInfo::VersionInfo(u8 verMajor, u8 verMinor, u8 verPatch) : major{ verMajor }, minor{ verMinor }, patch{ verPatch } {}