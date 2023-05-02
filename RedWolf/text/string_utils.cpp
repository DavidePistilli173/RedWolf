#include "string_utils.hpp"

using namespace rw::text;

std::string rw::text::leftTrim(std::string_view input)
{
   std::string retVal{ input };

   retVal.erase(retVal.begin(), std::find_if(retVal.begin(), retVal.end(), [](unsigned char c) { return !std::isspace(c); }));

   return retVal;
}

std::string rw::text::rightTrim(std::string_view input)
{
   std::string retVal{ input };

   retVal.erase(std::find_if(retVal.rbegin(), retVal.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), retVal.end());

   return retVal;
}

std::vector<std::string> rw::text::tokenize(std::string_view input, std::string_view separator)
{
   std::vector<std::string> retVal;

   size_t prevIdx{ 0U };
   size_t currentIdx{ input.find(separator) };

   while (currentIdx != std::string_view::npos)
   {
      retVal.emplace_back(trim(input.substr(prevIdx, currentIdx - prevIdx)));

      prevIdx = currentIdx;
      currentIdx = input.find(separator, currentIdx);
   }

   return retVal;
}

std::string rw::text::trim(std::string_view input)
{
   std::string retVal{ leftTrim(input) };
   retVal = rightTrim(input);
   return retVal;
}