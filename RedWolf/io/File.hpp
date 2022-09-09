#ifndef RW_IO_FILE_HPP
#define RW_IO_FILE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

namespace rw
{
   class RedWolfManager;
}

namespace rw::io
{
   /**
    * @brief Class for handling a single file.
    */
   class RW_API File
   {
   public:
      /**
       * @brief All possible modes to open a file.
       */
      enum class OpenMode
      {
         append,
         read,
         write
      };

      /**
       * @brief Different file formats for I/O operations.
       */
      enum class Format
      {
         bin,
         bmp,
         cab,
         dll,
         exe,
         ini,
         jpg,
         msi,
         png,
         sh,
         txt,
         xml,
         zip,

         unknown
      };

      /**
       * @brief Map for associating file extensions with their corresponding formats.
       */
      static constexpr rw::cont::ConstexprMap<std::string_view, Format, 32U> extension_map{
         { ".bin", Format::bin }, { ".bmp", Format::bmp }, { ".cab", Format::cab }, { ".dll", Format::dll }, { ".exe", Format::exe },
         { ".ini", Format::ini }, { ".jpg", Format::jpg }, { ".msi", Format::msi }, { ".png", Format::png }, { ".sh", Format::sh },
         { ".txt", Format::txt }, { ".xml", Format::xml }, { ".zip", Format::zip }
      };

      static constexpr char extension_separator{ '.' }; /**< Separator character for determining the file extension. */

      /**
       * @brief Constructor
       * @param manager RedWolf library manager.
       * @param filePath Path to the file.
       * @param mode Mode to open the file with.
       * @param format Format of the file. If unknown, the class will try to deduce it from the file extension.
       */
      File(RedWolfManager& manager, std::string_view filePath, OpenMode mode = OpenMode::read, Format format = Format::unknown);

      /**
       * @brief Extract the extension of the file.
       * @return File extension (from the first '.' onwards).
       */
      [[nodiscard]] std::string extension() const;

      /**
       * @brief Get the file format.
       * @return File format.
       */
      [[nodiscard]] Format format() const;

      /**
       * @brief Get the file path.
       * @return File path.
       */
      [[nodiscard]] std::filesystem::path path() const;

      /**
       * @brief Read all file contents and store them in a string.
       * @return File contents.
       */
      [[nodiscard]] std::string readAll() const;

      /**
       * @brief Read all file contents as binary data and cast them to a given object type.
       * @tparam T Data type to read.
       * @return Object evaluated with the file contents.
       */
      template<typename T>
      [[nodiscard]] T readAllBinary() const
      {
         if (!std::filesystem::exists(path_)) return T();

         std::ifstream f = std::ifstream(path_, std::ios_base::in | std::ios_base::binary);
         if (!f.is_open())
         {
            logger_->relErr("Failed to open file {}.", path_.string());
            return T();
         }

         T out;
         f.read(reinterpret_cast<char*>(&out), sizeof(out));

         if (f.gcount() < sizeof(T))
         {
            logger_->relErr("File too small for the selected object type (File {}; {}B/{}B).", path_.string(), f.gcount(), sizeof(T));
            return T();
         }

         return out;
      }

      /**
       * @brief Read a line from file.
       * @return Line read from file, if EOF was not reached.
       */
      [[nodiscard]] std::optional<std::string> readline();

   private:
      /**
       * @brief Extract the extension of the file from its name or full path.
       */
      void computeFormat_();

      rw::utils::Logger& logger_; /**< Logger instance. */

      std::filesystem::path path_;                      /**< Path to the file. */
      std::fstream          fileStream_;                /**< File stream. */
      Format                format_{ Format::unknown }; /**< Data format of the file. */
   };
} // namespace rw::io

/**
 * @brief std::formatter specialization for rw::io::File::Format
 */
template<>
struct std::formatter<rw::io::File::Format> : std::formatter<std::string>
{
   auto format(rw::io::File::Format format, std::format_context& ctx)
   {
      switch (format)
      {
      case rw::io::File::Format::bin:
         return std::formatter<std::string>::format("bin", ctx);
         break;
      case rw::io::File::Format::bmp:
         return std::formatter<std::string>::format("bmp", ctx);
         break;
      case rw::io::File::Format::cab:
         return std::formatter<std::string>::format("cab", ctx);
         break;
      case rw::io::File::Format::dll:
         return std::formatter<std::string>::format("dll", ctx);
         break;
      case rw::io::File::Format::exe:
         return std::formatter<std::string>::format("exe", ctx);
         break;
      case rw::io::File::Format::ini:
         return std::formatter<std::string>::format("ini", ctx);
         break;
      case rw::io::File::Format::jpg:
         return std::formatter<std::string>::format("jpg", ctx);
         break;
      case rw::io::File::Format::msi:
         return std::formatter<std::string>::format("msi", ctx);
         break;
      case rw::io::File::Format::png:
         return std::formatter<std::string>::format("png", ctx);
         break;
      case rw::io::File::Format::sh:
         return std::formatter<std::string>::format("sh", ctx);
         break;
      case rw::io::File::Format::txt:
         return std::formatter<std::string>::format("txt", ctx);
         break;
      case rw::io::File::Format::xml:
         return std::formatter<std::string>::format("xml", ctx);
         break;
      case rw::io::File::Format::zip:
         return std::formatter<std::string>::format("zip", ctx);
         break;
      case rw::io::File::Format::unknown:
         return std::formatter<std::string>::format("unknown", ctx);
         break;
      }
      return std::formatter<std::string>::format("INVALID", ctx);
   }
};

#endif