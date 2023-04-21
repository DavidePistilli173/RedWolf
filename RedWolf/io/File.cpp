#include "File.hpp"

#include <fstream>

using namespace rw::io;

File::File(rw::util::Logger& logger, std::string_view filePath, OpenMode mode, Format format) :
   logger_{ logger }, path_{ filePath }, format_{ format }
{
   if (std::filesystem::exists(path_) && std::filesystem::is_directory(path_))
   {
      logger_.relFatal("The input path refers to a directory, not a file ({}).", path_.string());
   }

   if (format == Format::unknown) computeFormat_();

   // Check whether the file is in a binary or text format.
   bool binary{ false };
   switch (format_)
   {
   case Format::bin:
   case Format::bmp:
   case Format::cab:
   case Format::dll:
   case Format::exe:
   case Format::jpg:
   case Format::msi:
   case Format::png:
   case Format::zip:
      binary = true;
      break;
   case Format::ini:
   case Format::sh:
   case Format::txt:
   case Format::xml:
      binary = false;
      break;
   }

   switch (mode)
   {
   case OpenMode::append:
      if (binary)
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::app | std::ios_base::binary);
      }
      else
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::app);
      }
      break;
   case OpenMode::read:
      if (binary)
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::in | std::ios_base::binary);
      }
      else
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::in);
      }
      break;
   case OpenMode::write:
      if (binary)
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::out | std::ios_base::binary);
      }
      else
      {
         fileStream_ = std::fstream(filePath.data(), std::ios_base::out);
      }
      break;
   }
}

std::string File::extension() const
{
   return path_.extension().string();
}

File::Format File::format() const
{
   return format_;
}

bool File::isOpen() const
{
   return fileStream_.is_open();
}

std::filesystem::path File::path() const
{
   return path_;
}

std::vector<std::byte> File::readAllByteArray() const
{
   if (!std::filesystem::exists(path_)) return std::vector<std::byte>();

   std::ifstream f{ std::ifstream(path_, std::ios_base::in | std::ios_base::binary | std::ios_base::ate) };
   if (!f.is_open())
   {
      logger_.relErr("Failed to open file {}.", path_.string());
      return std::vector<std::byte>();
   }

   size_t                 fileSize{ static_cast<size_t>(f.tellg()) };
   std::vector<std::byte> out(fileSize);
   f.seekg(0U);
   f.read(reinterpret_cast<char*>(out.data()), out.size());

   return out;
}

std::string File::readAllString() const
{
   if (!std::filesystem::exists(path_)) return std::string();

   std::ifstream f{ std::ifstream(path_, std::ios_base::in | std::ios_base::ate) };
   if (!f.is_open())
   {
      logger_.relErr("Failed to open file {}.", path_.string());
      return std::string();
   }

   size_t      fileSize{ static_cast<size_t>(f.tellg()) };
   std::string out(fileSize, '\0');
   f.seekg(0U);
   f.read(out.data(), out.size());

   return out;
}

std::optional<std::string> File::readline()
{
   std::string retVal;
   if (std::getline(fileStream_, retVal))
   {
      return retVal;
   }

   return std::optional<std::string>();
}

void File::computeFormat_()
{
   std::string lowerCaseExtension{ path_.extension().string() };
   for (auto& c : lowerCaseExtension)
   {
      c = static_cast<char>(std::tolower(c));
   }

   auto extIt = extension_map.find(lowerCaseExtension);
   format_ = extIt == extension_map.end() ? Format::unknown : extIt->value;
}