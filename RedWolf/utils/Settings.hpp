#ifndef RW_UTILS_SETTINGS_HPP
#define RW_UTILS_SETTINGS_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/SettingsNode.hpp"
#include "RedWolf/io/File.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <string_view>
#include <utility>

namespace rw::utils
{
   /**
    * @brief Class for loading and managing application-wide settings in a thread-safe manner.
    */
   class Settings
   {
   public:
      using Node = rw::dat::SettingsNode;

      static constexpr std::string_view default_settings_file{ "./config/config.ini" }; /**< Default settings file. */
      static constexpr size_t           max_settings_files{ 64U };                      /**< Maximum number of settings files. */

      /**
       * @brief Get the instance of the Settings class.
       * @param filePath Path of the settings file to use.
       * @param format Format of the settings file.
       * @return Settings* Pointer to the settings instance.
       */
      static Settings*
         instance(std::string_view filePath = default_settings_file, rw::io::File::Format format = rw::io::File::Format::unknown);

   private:
      /**
       * @brief Constructor.
       */
      Settings();

      /**
       * @brief Load settings from a file.
       * @param filePath Path to the file.
       * @param format Input file format. If set to unknown, it will be deduced from the file extension.
       * @return Newly inserted settings instance.
       */
      static std::array<rw::dat::KVPair<std::string, std::unique_ptr<Settings>>, max_settings_files>::iterator
         load_(std::string_view filePath = default_settings_file, rw::io::File::Format format = rw::io::File::Format::unknown);

      /**
       * @brief Load an INI settings file.
       * @param file File to load.
       * @return Iterator to the newly inserted settings instance.
       */
      static std::array<rw::dat::KVPair<std::string, std::unique_ptr<Settings>>, max_settings_files>::iterator loadIni_(rw::io::File& file);

      /**
       * @brief Settings instances for each settings file.
       */
      static std::array<rw::dat::KVPair<std::string, std::unique_ptr<Settings>>, max_settings_files> instances_;
      static std::mutex mtx_;         /**< Mutex for protecting changes to the instances vector. */
      static size_t     activeFiles_; /**< Number of active settings files. */

      Logger*               logger_{ nullptr }; /**< Logger. */
      std::unique_ptr<Node> root_{ nullptr };   /**< Root node of the settings tree. */
   };
} // namespace rw::utils

#endif