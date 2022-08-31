#ifndef RW_TEXT_STRING_UTILS_HPP
#define RW_TEXT_STRING_UTILS_HPP

#include "RedWolf/common.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace rw::text
{
   /**
    * @brief Trim the beginning of a string, removing leading whitespaces.
    * @param input String to trim.
    * @return Trimmed string.
    */
   RW_API std::string leftTrim(std::string_view input);

   /**
    * @brief Trim the end of a string, removing trailing whitespaces.
    * @param input String to trim.
    * @return Trimmed string.
    */
   RW_API std::string rightTrim(std::string_view input);

   /**
    * @brief Tokenize a string considering a given separator pattern.
    * @param input Input string to tokenize.
    * @param separator Separator pattern for the tokenization process.
    * @return Vector of all trimmed tokens.
    */
   RW_API std::vector<std::string> tokenize(std::string_view input, std::string_view separator);

   /**
    * @brief Trim a string, removing leading and trailing whitespaces.
    * @param input String to trim.
    * @return Trimmed string.
    */
   RW_API std::string trim(std::string_view input);
} // namespace rw::text

#endif