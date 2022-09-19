#ifndef RW_LIF_VULKAN_SWAPCHAIN_HPP
#define RW_LIF_VULKAN_SWAPCHAIN_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class RW_API SwapChain
   {
   public:
   private:
      Surface& surface_; /**< Surface that the swap chain is referred to. */
   };
} // namespace rw::lif::vlk

#endif