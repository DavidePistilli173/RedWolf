#ifndef RW_IO_CUSTOMFORMATTERS_HPP
#define RW_IO_CUSTOMFORMATTERS_HPP

#include "RedWolf/io/concepts.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <vector>

/**
 * @brief std::formatter specialization for std::vector<std::byte>.
 */
template<>
struct std::formatter<std::vector<std::byte>> : std::formatter<std::string>
{
   auto format(const std::vector<std::byte>& data, std::format_context& ctx)
   {
      const std::string result{ reinterpret_cast<const char*>(data.data()), data.size() };
      return std::formatter<std::string>::format(result, ctx);
   }
};

/**
 * @brief std::formatter specialization for pointer types.
 */
template<rw::io::IsPointer T>
struct std::formatter<T> : std::formatter<void*>
{
   auto format(const T& data, std::format_context& ctx)
   {
      return std::formatter<void*>::format(reinterpret_cast<void*>(data), ctx);
   }
};

#endif