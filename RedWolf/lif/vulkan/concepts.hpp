#ifndef RW_LIF_VULKAN_CONCEPTS_HPP
#define RW_LIF_VULKAN_CONCEPTS_HPP

#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

#include <concepts>
#include <type_traits>

namespace rw::lif::vlk
{
   template<typename T>
   concept IsQueueFamilyIdList = requires(T list)
   {
      std::is_same_v<decltype(list[0]), QueueFamilies::Id>;
   };
} // namespace rw::lif::vlk

#endif