#ifndef RW_IO_FILE_HPP
#define RW_IO_FILE_HPP

#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <filesystem>
#include <string>
#include <string_view>

namespace rw::io
{
   /**
    * @brief Class for handling a single file.
    */
   class File
   {
   public:
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
       * @param filePath Path to the file.
       * @param format Format of the file. If unknown, the class will try to deduce it from the file extension.
       */
      File(std::string_view filePath, Format format = Format::unknown);

      /**
       * @brief Extract the extension of the file.
       * @return File extension (from the first '.' onwards).
       */
      [[nodiscard]] std::string extension() const;

      /**
       * @brief Get the file format.
       */
      [[nodiscard]] Format format() const;

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
            logger_->relErr("File::readAllBinary - Failed to open file {}.", path_.string());
            return T();
         }

         T out;
         f.read(reinterpret_cast<char*>(&out), sizeof(out));

         if (f.gcount() < sizeof(T))
         {
            logger_->relErr(
               "File::readAllBinary - File too small for the selected object type (File {}; {}B/{}B).",
               path_.string(),
               f.gcount(),
               sizeof(T));
            return T();
         }

         return out;
      }

   private:
      /**
       * @brief Extract the extension of the file from its name or full path.
       */
      void computeFormat_();

      mutable rw::utils::Logger* logger_{ nullptr }; /**< Logger instance. */

      std::filesystem::path path_;                      /**< Path to the file. */
      Format                format_{ Format::unknown }; /**< Data format of the file. */
   };
} // namespace rw::io

#endif