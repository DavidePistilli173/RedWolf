#ifndef RW_LIF_VULKAN_BASEOBJECT_HPP
#define RW_LIF_VULKAN_BASEOBJECT_HPP

#include "RedWolf/common.hpp"

#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::utils
{
   class Logger;
} // namespace rw::utils

namespace rw::lif::vlk
{
   class Instance;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Base class for all Vulkan objects (except for the instance).
    */
   class RW_API BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      explicit BaseObject(RedWolfManager& manager);

      /**
       * @brief Destructor.
       */
      virtual ~BaseObject() = default;

      /**
       * @brief Copy constructor.
       */
      BaseObject(const BaseObject&) = default;

      /**
       * @brief Move constructor.
       */
      BaseObject(BaseObject&&) noexcept = default;

      /**
       * @brief Copy-assignment operator.
       */
      BaseObject& operator=(const BaseObject&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      BaseObject& operator=(BaseObject&&) = delete;

   protected:
      RedWolfManager&    manager_;        /**< RedWolf library manager. */
      rw::utils::Logger& logger_;         /**< Library logger. */
      Instance&          vulkanInstance_; /**< Library Vulkan instance. */
   };
} // namespace rw::lif::vlk

#endif