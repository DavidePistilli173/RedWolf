#ifndef RW_LIF_VULKAN_SWAPCHAIN_HPP
#define RW_LIF_VULKAN_SWAPCHAIN_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"

#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class Surface;
   class PhysicalDevice;
   class GraphicsDevice;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   class RW_API SwapChain : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param physicalDevice Physical device that will present images to the swap chain.
       * @param graphicsDevice Graphics device that will present images to the swap chain.
       * @param surface Surface this swap chain is bound to.
       */
      SwapChain(RedWolfManager& manager, PhysicalDevice& physicalDevice, GraphicsDevice& graphicsDevice, Surface& surface);

      /**
       * @brief Destructor.
       */
      ~SwapChain();

   private:
      GraphicsDevice& graphicsDevice_; /**< Logical device this swap chain is bound to. */

      VkSwapchainKHR       swapChain_{ VK_NULL_HANDLE }; /**< Raw swap chain handle. */
      std::vector<VkImage> images_;                      /**< Handles to the swap chain's images. */
   };
} // namespace rw::lif::vlk

#endif