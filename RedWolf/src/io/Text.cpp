#include "RedWolf/io/Text.hpp"

#include "RedWolf/Log.hpp"
#include "RedWolf/utility.hpp"

#include <SDL2.0.9/SDL.h>
#include <array>

namespace rw::io
{
   /**
      \param file Text file to load.
   */
   std::string Text::load(std::string_view file)
   {
      // Open the file.
      rw::file_ptr_t fileHandle{
         SDL_RWFromFile(file.data(), "r"),
         [](SDL_RWops* file) { SDL_RWclose(file); }
      };
      if (fileHandle == nullptr)
      {
         Log::core_err("Failed to open file \"{1}\".\n   {2}", file, SDL_GetError());
         return std::string{};
      }

      // Reserve space for the file contents.
      std::string contents;
      contents.reserve(SDL_RWsize(fileHandle.get()));

      // Read the contents of the file.
      std::array<char, read_chunk> readBuf;
      bool                         done{ false };
      while (!done)
      {
         // If EOF is reached and no characters are read, null terminate the contents and stop.
         if (size_t charRead{ SDL_RWread(fileHandle.get(), readBuf.data(), sizeof(char), read_chunk) };
             charRead == 0)
         {
            contents += '\0'; // Null terminate the string.
            done = true;
         }
         else
         {
            // If EOF is reached and some characters are read, null terminate the buffer.
            if (charRead < read_chunk)
            {
               readBuf[charRead] = '\0';
               done = true;
            }
            // Update the file contents.
            contents += readBuf.data();
         }
      }

      return contents;
   }
} // namespace rw::io