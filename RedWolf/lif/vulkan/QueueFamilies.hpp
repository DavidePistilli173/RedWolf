#ifndef RW_LIF_VULKAN_VULKANQUEUEFAMILIES_HPP
#define RW_LIF_VULKAN_VULKANQUEUEFAMILIES_HPP

#include "RedWolf/common.hpp"

#include <optional>

namespace rw::lif::vlk
{
   /**
    * @brief Container for storing the indices of a device's queue families.
    */
   struct RW_API QueueFamilies
   {
      std::optional<uint32_t> graphics;     /**< Queue family for graphics commands. */
      std::optional<uint32_t> presentation; /**< Queue family for image presentation to surfaces. */
   };
} // namespace rw::lif::vlk
#endif