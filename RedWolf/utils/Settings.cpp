#include "Settings.hpp"

#include "RedWolf/text/string_utils.hpp"

#include <algorithm>

std::array<rw::dat::KVPair<std::string, std::unique_ptr<rw::utils::Settings>>, rw::utils::Settings::max_settings_files>
           rw::utils::Settings::instances_;
std::mutex rw::utils::Settings::mtx_;
size_t     rw::utils::Settings::activeFiles_{ 0U };

namespace rw::utils
{
   Settings::Settings(RedWolfManager& manager) : logger_{ manager.logger() } {}

   Settings* Settings::instance(RedWolfManager& manager, std::string_view filePath, rw::io::File::Format format)
   {
      Logger& logger{ manager.logger() };

      std::scoped_lock lck{ mtx_ };

      if (activeFiles_ == max_settings_files)
      {
         logger.relErr("Maximum number of settings files reached: {}.", max_settings_files);
         throw std::exception{ "Maximum number of settings files reached." };
      }

      auto instanceIt = std::find_if(
         instances_.begin(),
         instances_.end(),
         [filePath, &logger](const rw::dat::KVPair<std::string, std::unique_ptr<Settings>>& element) { return element.key == filePath; });
      if (instanceIt == instances_.end())
      {
         instanceIt = load_(manager, filePath, format);
         logger.relInfo("Created settings instance {}.", filePath);
      }

      logger.trace("Providing settings instance.");
      return instanceIt->value.get();
   }

   Settings::Node* Settings::root()
   {
      return root_.get();
   }

   std::array<rw::dat::KVPair<std::string, std::unique_ptr<Settings>>, Settings::max_settings_files>::iterator
      Settings::load_(RedWolfManager& manager, std::string_view filePath, rw::io::File::Format format)
   {
      Logger& logger{ manager.logger() };

      rw::io::File file{ manager, filePath, rw::io::File::OpenMode::read, format };

      if (format == rw::io::File::Format::unknown)
      {
         format = file.format();
      }

      switch (format)
      {
      case rw::io::File::Format::ini:
         return loadIni_(manager, file);
         break;
      case rw::io::File::Format::xml:
      default:
         logger.relFatal("Unsupported file format {} for settings file {}.", format, filePath);
         break;
      }
   }

   std::array<rw::dat::KVPair<std::string, std::unique_ptr<Settings>>, Settings::max_settings_files>::iterator
      Settings::loadIni_(RedWolfManager& manager, rw::io::File& file)
   {
      instances_[activeFiles_].key = file.path().string();
      instances_[activeFiles_].value = std::unique_ptr<Settings>(new Settings(manager));
      instances_[activeFiles_].value->root_ = std::make_unique<Node>(nullptr, "General", "");
      Node* generalSection = instances_[activeFiles_].value->root_.get();
      Node* activeSection = instances_[activeFiles_].value->root_.get();

      for (auto val = file.readline(); val.has_value(); val = file.readline())
      {
         std::string trimmedLine = val.value();

         // Remove comments
         size_t commentIdx{ trimmedLine.find('#') };
         if (commentIdx != std::string::npos)
         {
            trimmedLine = trimmedLine.substr(0U, commentIdx);
         }

         // Trim the line.
         trimmedLine = rw::text::trim(trimmedLine);
         if (!trimmedLine.empty())
         {
            // Check new section start.
            if (trimmedLine[0] == '[' && trimmedLine[trimmedLine.length() - 1] == ']')
            {
               activeSection = generalSection->addChild(generalSection, trimmedLine.substr(1U, trimmedLine.length() - 2), "");
            }
            // Add the attribute to the current section.
            else
            {
               size_t equalsIdx{ trimmedLine.find('=') };
               if (equalsIdx != std::string::npos)
               {
                  std::string attributeName{ rw::text::rightTrim(trimmedLine.substr(0U, equalsIdx)) };
                  std::string attributeValue{ rw::text::leftTrim(trimmedLine.substr(equalsIdx + 1U, trimmedLine.length())) };
                  activeSection->setAttribute(attributeName, attributeValue);
               }
            }
         }
      }

      ++activeFiles_;
      return instances_.begin() + activeFiles_ - 1;
   }
} // namespace rw::utils