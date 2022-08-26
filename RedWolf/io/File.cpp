#include "File.hpp"

#include <fstream>

namespace rw::io
{
   File::File(std::string_view filePath, Format format) : logger_{ rw::utils::Logger::instance() }, path_{ filePath }, format_{ format }
   {
      if (std::filesystem::exists(path_) && std::filesystem::is_directory(path_))
      {
         logger_->relErr("The input path refers to a directory, not a file ({}).", path_.string());
         throw std::exception{ "The input path refers to a directory, not a file." };
      }

      if (format == Format::unknown) computeFormat_();
   }

   std::string File::extension() const
   {
      return path_.extension().string();
   }

   File::Format File::format() const
   {
      return format_;
   }

   std::string File::readAll() const
   {
      static constexpr size_t read_size = 4096U;

      if (!std::filesystem::exists(path_)) return std::string();

      std::ifstream f = std::ifstream(path_, std::ios_base::in);
      if (!f.is_open())
      {
         logger_->relErr("Failed to open file {}.", path_.string());
         return std::string();
      }

      std::string out;
      std::string buf = std::string(read_size, '\0');
      while (f.read(&buf[0], read_size))
      {
         out.append(buf, 0, f.gcount());
      }
      out.append(buf, 0, f.gcount());
      return out;
   }

   void File::computeFormat_()
   {
      std::string lowerCaseExtension{ path_.extension().string() };
      for (auto& c : lowerCaseExtension)
      {
         c = std::tolower(c);
      }

      auto extIt = extension_map.find(lowerCaseExtension);
      format_ = extIt == extension_map.end() ? Format::unknown : extIt->value;
   }
} // namespace rw::io