#ifndef RW_LIF_VULKAN_CONCEPTS_HPP
#define RW_LIF_VULKAN_CONCEPTS_HPP

#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"

#include <concepts>
#include <iterator>
#include <type_traits>

namespace rw::lif::vlk
{
   /**
    * @brief Concept for a contiguous list of QueueFamilies IDs.
    */
   template<typename T>
   concept IsQueueFamilyIdArray =
      rw::cont::IsContiguousContainer<T> && requires(T list) { std::is_same_v<decltype(list[0]), QueueFamilies::Id>; };

   /**
    * @brief Concept for a contiguous list of VkFence objects.
    */
   template<typename T>
   concept IsVkFenceArray = rw::cont::IsContiguousContainer<T> && requires(T list) { std::is_same_v<decltype(list[0]), VkFence>; };
} // namespace rw::lif::vlk

#endif