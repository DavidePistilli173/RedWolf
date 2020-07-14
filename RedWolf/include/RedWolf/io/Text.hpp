#ifndef RW_IO_TEXT_HPP
#define RW_IO_TEXT_HPP

#include "RedWolf/core.hpp"

#include <string>
#include <string_view>

namespace rw::io
{
   /** \brief Read and write operations for text files. */
   class RW_API Text
   {
   public:
      /** \brief Number of characters to read in one operation. */
      static constexpr size_t read_chunk{ 4096 };

      /** \brief Load the contents of a text file into a string. */
      static std::string load(std::string_view file);
   };
} // namespace rw::io

#endif