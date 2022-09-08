#ifndef RW_LIBIF_VULKANMANAGER_HPP
#define RW_LIBIF_VULKANMANAGER_HPP

#include "RedWolf/utils/Logger.hpp"

#include <mutex>
#include <string_view>

// Forward declarations of Vulkan types.
struct VkInstance_T;
using VkInstance = VkInstance_T*;

struct VkDebugUtilsMessengerEXT_T;
using VkDebugUtilsMessengerEXT = VkDebugUtilsMessengerEXT_T*;

namespace rw::libif
{
   /**
    * @brief Interface for the Vulkan library.
    */
   class RW_API VulkanManager
   {
   public:
      /**
       * @brief Constructor.
       * @param appName Name of the application.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       */
      VulkanManager(std::string_view appName, int verMajor, int verMinor, int verPatch);

      /**
       * @brief Destructor.
       */
      ~VulkanManager();

   private:
      /**
       * @brief Create the Vulkan instance.
       * @param appName Name of the application.
       * @param verMajor Major version number.
       * @param verMinor Minor version number.
       * @param verPatch Patch version number.
       */
      void createInstance_(std::string_view appName, int verMajor, int verMinor, int verPatch);

      /**
       * @brief Initialise all Vulkan function pointers.
       */
      void initialiseFunctions_();

      static std::mutex mtx_;         /**< Mutex for protecting concurrent access to the library. */
      static size_t     activeUsers_; /**< Number of currently active users. */

      static VkInstance               instance_;       /**< Vulkan instance. */
      static VkDebugUtilsMessengerEXT debugMessenger_; /**< Vulkan debug information. */

      rw::utils::Logger* logger_; /**< Logger instance. */
   };
} // namespace rw::libif

#endif