#ifndef RW_LIF_VULKAN_VULKANQUEUEFAMILIES_HPP
#define RW_LIF_VULKAN_VULKANQUEUEFAMILIES_HPP

#include "RedWolf/common.hpp"

#include <array>
#include <optional>

namespace rw::lif::vlk
{
   /**
    * @brief Container for storing the indices of a device's queue families.
    */
   struct RW_API QueueFamilies
   {
      /**
       * @brief Queue IDs.
       */
      enum class Id
      {
         graphics,     /**< Queue family for graphics commands. */
         presentation, /**< Queue family for image presentation to surfaces. */
         num_elements
      };

      std::array<std::optional<u32>, static_cast<size_t>(Id::num_elements)> queues; /**< Queue indices. */
   };
} // namespace rw::lif::vlk
#endif