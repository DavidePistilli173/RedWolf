#ifndef RW_LIF_VULKAN_FUNCTIONPOINTERS_HPP
#define RW_LIF_VULKAN_FUNCTIONPOINTERS_HPP

#include <vulkan/vulkan.h>

namespace rw::lif::vlk
{
   /**
    * @brief Container for all Vulkan function pointers.
    */
   struct FunctionPointers
   {
      PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT{ nullptr };
      PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT{ nullptr };
   };
} // namespace rw::lif::vlk

#endif