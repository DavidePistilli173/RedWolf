#ifndef RW_LIF_VULKAN_SWAPCHAIN_HPP
#define RW_LIF_VULKAN_SWAPCHAIN_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/ImageView.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"

#include <vector>

namespace rw
{
   class RedWolfManager;
} // namespace rw

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
      ~SwapChain() override;

      /**
       * @brief Copy constructor.
       */
      SwapChain(const SwapChain&) = delete;

      /**
       * @brief Move constructor.
       */
      SwapChain(SwapChain&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      SwapChain& operator=(const SwapChain&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      SwapChain& operator=(SwapChain&&) = delete;

      /**
       * @brief Get the current format of the swap chain.
       * @return Current format of the swap chain.
       */
      [[nodiscard]] const VkSurfaceFormatKHR& format() const;

   private:
      VkSwapchainKHR     swapChain_{ VK_NULL_HANDLE }; /**< Raw swap chain handle. */
      VkSurfaceFormatKHR format_;                      /**< Current format of the swap chain. */
      VkPresentModeKHR   mode_;                        /**< Current mode of the swap chain. */
      VkExtent2D         extent_;                      /**< Current extent of the swap chain. */

      GraphicsDevice& graphicsDevice_; /**< Logical device this swap chain is bound to. */

      std::vector<ImageView> images_; /**< Swap chain images. */
   };
} // namespace rw::lif::vlk

#endif