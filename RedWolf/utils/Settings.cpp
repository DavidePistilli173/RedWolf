#include "Settings.hpp"

#include <algorithm>

std::array<rw::dat::KVPair<std::string, rw::dat::MVPair<std::unique_ptr<rw::utils::Settings>>>, rw::utils::Settings::max_settings_files>
           rw::utils::Settings::instances_;
std::mutex rw::utils::Settings::mtx_;
size_t     rw::utils::Settings::activeFiles_{ 0U };

namespace rw::utils
{
   Settings::Settings() : logger_{ Logger::instance() } {}

   Settings* Settings::instance(std::string_view filePath, rw::io::File::Format format)
   {
      Logger* logger{ Logger::instance() };

      std::scoped_lock lck{ mtx_ };

      if (activeFiles_ == max_settings_files)
      {
         logger->relErr("Settings::instance - Maximum number of settings files reached: {}.", max_settings_files);
         throw std::exception{ "Settings::instance - Maximum number of settings files reached." };
      }

      auto instanceIt = std::find_if(
         instances_.begin(),
         instances_.end(),
         [filePath, logger](const rw::dat::KVPair<std::string, rw::dat::MVPair<std::unique_ptr<Settings>>>& element)
         { return element.key == filePath; });
      if (instanceIt == instances_.end())
      {
         instanceIt = load_(filePath, format);
         logger->relInfo("Settings::instance - created settings instance {}.", filePath);
      }

      logger->trace("Settings::instance - Providing settings instance.");
      return instanceIt->value.value.get();
   }

   std::array<rw::dat::KVPair<std::string, rw::dat::MVPair<std::unique_ptr<Settings>>>, rw::utils::Settings::max_settings_files>::iterator
      Settings::load_(std::string_view filePath, rw::io::File::Format format)
   {
      rw::io::File file{ filePath, format };
      ++activeFiles_;
   }
} // namespace rw::utils