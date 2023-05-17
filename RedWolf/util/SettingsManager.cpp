#include "SettingsManager.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/text/string_utils.hpp"

#include <algorithm>

using namespace rw::util;

SettingsManager::SettingsManager(rw::util::Logger& logger) : logger_{ logger } {}

SettingsManager::Node* SettingsManager::get(std::string_view filePath, rw::io::File::Format format)
{
   std::scoped_lock lck{ mtx_ };

   if (activeFiles_ == max_settings_files)
   {
      logger_.relErr("Maximum number of settings files reached: {}.", max_settings_files);
      throw std::exception{ "Maximum number of settings files reached." };
   }

   auto instanceIt = std::find_if(
      instances_.begin(),
      instances_.end(),
      [filePath, this](const rw::dat::KVPair<std::string, std::unique_ptr<Node>>& element) { return element.key == filePath; });
   if (instanceIt == instances_.end())
   {
      instanceIt = load_(filePath, format);
      logger_.relInfo("Created settings instance {}.", filePath);
   }

   logger_.trace("Providing settings instance.");
   return instanceIt->value.get();
}

std::array<rw::dat::KVPair<std::string, std::unique_ptr<SettingsManager::Node>>, SettingsManager::max_settings_files>::iterator
   SettingsManager::load_(std::string_view filePath, rw::io::File::Format format)
{
   rw::io::File file{ logger_, filePath, rw::io::File::OpenMode::read, format };

   if (format == rw::io::File::Format::unknown)
   {
      format = file.format();
   }

   switch (format)
   {
   case rw::io::File::Format::ini:
      return loadIni_(file);
      break;
   case rw::io::File::Format::xml:
   default:
      logger_.relFatal("Unsupported file format {} for settings file {}.", format, filePath);
      break;
   }
}

std::array<rw::dat::KVPair<std::string, std::unique_ptr<SettingsManager::Node>>, SettingsManager::max_settings_files>::iterator
   SettingsManager::loadIni_(rw::io::File& file)
{
   instances_[activeFiles_].key = file.path().string();
   instances_[activeFiles_].value = std::unique_ptr<Node>(new Node(nullptr, "General", ""));
   Node* generalSection = instances_[activeFiles_].value.get();
   Node* activeSection = instances_[activeFiles_].value.get();

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