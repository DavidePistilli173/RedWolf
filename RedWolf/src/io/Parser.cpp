#include "RedWolf/io/Parser.hpp"

#include <algorithm>

namespace rw::io
{
   /**
      \param text View on the text to parse.
   */
   Parser::Parser(std::string_view text) :
      memory_{ new std::byte[mem_size] },
      memRes_{ memory_.get(), mem_size },
      text_{ text, &memRes_ },
      index_{ &memRes_ }
   {
   }

   /**
      \param pattern Pattern to look for.
      \param pos Starting search position.
      \return First index of the pattern.
   */
   size_t Parser::find(std::string_view pattern, size_t pos)
   {
      // If the search was already performed, return the stored result.
      auto indexElem = index_.find(pattern.data());
      if (indexElem != index_.end())
      {
         if (auto idx = std::lower_bound(indexElem->second.cbegin(), indexElem->second.cend(), pos);
             idx != indexElem->second.cend())
         {
            return *idx;
         }
      }
      else
      {
         indexElem = index_.insert(std::make_pair(pattern, std::set<size_t>{})).first;
      }

      // If no results are available, parse the text.
      size_t target{ text_.find(pattern, pos) };
      indexElem->second.insert(target);
      return target;
   }

   /**
      \param pattern Pattern to look for.
      \return Ordered set of all occurences.
   */
   std::set<size_t> Parser::findAll(std::string_view pattern)
   {
      auto indexElem = index_.find(pattern.data());

      size_t startPos{ 0 };
      if (indexElem != index_.end())
         startPos = *(std::max_element(indexElem->second.cbegin(), indexElem->second.cend())) + 1;
      else
         indexElem = index_.insert(std::make_pair(pattern, std::set<size_t>{})).first;

      while (startPos < text_.size() && startPos != std::string_view::npos)
      {
         startPos = text_.find(pattern, startPos);
         if (startPos != std::string_view::npos)
         {
            indexElem->second.insert(startPos);
            ++startPos;
         }
      }

      return indexElem->second;
   }
} // namespace rw::io