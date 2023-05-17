#ifndef RW_UTIL_SETTINGSMANAGER_HPP
#define RW_UTIL_SETTINGSMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/SettingsNode.hpp"
#include "RedWolf/io/File.hpp"
#include "RedWolf/util/Logger.hpp"

#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <string_view>
#include <utility>

namespace rw::util
{
   /**
    * @brief Class for loading and managing application-wide settings in a thread-safe manner.
    */
   class RW_API SettingsManager
   {
   public:
      using Node = rw::dat::SettingsNode;

      static constexpr std::string_view default_settings_file{ "./config/config.ini" }; /**< Default settings file. */
      static constexpr size_t           max_settings_files{ 64U };                      /**< Maximum number of settings files. */

      /**
       * @brief Constructor.
       * @param logger Logger.
       */
      explicit SettingsManager(rw::util::Logger& logger);

      /**
       * @brief Get the root of a given settings tree.
       * @param filePath Path of the settings file to use.
       * @param format Format of the settings file.
       * @return Pointer to the root of the settings tree.
       */
      Node* get(std::string_view filePath = default_settings_file, rw::io::File::Format format = rw::io::File::Format::unknown);

   private:
      /**
       * @brief Load settings from a file.
       * @param filePath Path to the file.
       * @param format Input file format. If set to unknown, it will be deduced from the file extension.
       * @return Newly inserted settings instance.
       */
      std::array<rw::dat::KVPair<std::string, std::unique_ptr<Node>>, max_settings_files>::iterator
         load_(std::string_view filePath = default_settings_file, rw::io::File::Format format = rw::io::File::Format::unknown);

      /**
       * @brief Load an INI settings file.
       * @param file File to load.
       * @return Iterator to the newly inserted settings instance.
       */
      std::array<rw::dat::KVPair<std::string, std::unique_ptr<Node>>, max_settings_files>::iterator loadIni_(rw::io::File& file);

      rw::util::Logger& logger_; /**< Logger. */

      /**
       * @brief Settings instances for each settings file.
       */
      std::array<rw::dat::KVPair<std::string, std::unique_ptr<Node>>, max_settings_files> instances_;
      std::mutex mtx_;               /**< Mutex for protecting changes to the instances vector. */
      size_t     activeFiles_{ 0U }; /**< Number of active settings files. */
   };
} // namespace rw::util

#endif