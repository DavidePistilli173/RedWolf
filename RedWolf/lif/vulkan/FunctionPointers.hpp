#ifndef RW_LIF_VULKAN_FUNCTIONPOINTERS_HPP
#define RW_LIF_VULKAN_FUNCTIONPOINTERS_HPP

#include <vulkan/vulkan.h>

namespace rw::lif::vulkan
{
   /**
    * @brief Container for all Vulkan function pointers.
    */
   struct FunctionPointers
   {
      PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT{ nullptr };
      PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT{ nullptr };
   };
} // namespace rw::lif::vulkan

#endif