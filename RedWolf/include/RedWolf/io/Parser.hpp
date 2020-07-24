#include "RedWolf/core.hpp"

#include <array>
#include <map>
#include <memory_resource>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace rw::io
{
   /** \brief Text parser. */
   class RW_API Parser
   {
   public:
      /** \brief Delimiters for uniform parsing in GLSL shaders. */
      static constexpr std::array glsl_ufrm_delims{
         ' ', '\n', '\r'
      };
      /** \brief Endline character for uniform parsing in GLSL shaders. */
      static constexpr char glsl_ufrm_endln{ ';' };

      /** \brief Size of a single chunk of memory used by the parser. */
      static constexpr size_t mem_size{ 8192 };

      /** \brief Create a text parser. */
      explicit Parser(std::string_view text);

      /** \brief Find the first occurence of a specific pattern from a given position. */
      [[nodiscard]] size_t find(std::string_view pattern, size_t pos = 0);

      /** \brief Find all occurences of a specific pattern. */
      [[nodiscard]] std::set<size_t> findAll(std::string_view pattern);

      /** 
         \brief Tokenise the text from the given position up to the first endline. 
         \param delimiters Single character delimiters for the tokens.
         \param endLine Endline character that ends the tokenisation.
         \param pos Starting position for tokenisation.
         \return Tokens in the line.
      */
      template<size_t S>
      [[nodiscard]] std::vector<std::string_view> tokeniseLine(
         const std::array<char, S>& delimiters,
         char                       endLine,
         size_t                     pos = 0)
      {
         std::vector<std::string_view> result;
         bool                          startOnWord{ false };
         size_t                        start{ pos };

         while (pos < text_.size() && text_[pos] != endLine)
         {
            if (auto delimiterMatch = std::find(delimiters.cbegin(), delimiters.cend(), text_[pos]);
                delimiterMatch != delimiters.cend())
            {
               if (startOnWord)
               {
                  result.emplace_back(&text_[start], pos - start);
                  startOnWord = false;
               }
            }
            else
            {
               if (!startOnWord)
               {
                  start = pos;
                  startOnWord = true;
               }
            }
            ++pos;
         }
         result.emplace_back(&text_[start], pos - start);

         return result;
      }

   private:
      /** \brief Raw memory for the stored objects. */
      std::unique_ptr<std::byte> memory_;
      /** \brief PMR for the stored objects. */
      std::pmr::monotonic_buffer_resource memRes_;
      /** \brief Input text. */
      std::pmr::string text_;
      /** \brief Map of all strings that were looked for with their positions. */
      std::pmr::map<std::string, std::set<size_t>> index_;
   };
} // namespace rw::io