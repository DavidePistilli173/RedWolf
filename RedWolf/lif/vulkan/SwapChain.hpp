#ifndef RW_LIF_VULKAN_SWAPCHAIN_HPP
#define RW_LIF_VULKAN_SWAPCHAIN_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/ImageView.hpp"

#include <vector>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   class Fence;
   class DeviceBase;
   class PhysicalDevice;
   class RenderPass;
   class Semaphore;
   class Surface;
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
       * @param device Logical device that will present images to the swap chain.
       * @param surface Surface this swap chain is bound to.
       * @param oldSwapchain Swapchain that needs to be replaced.
       */
      SwapChain(
         RedWolfManager&       manager,
         const PhysicalDevice& physicalDevice,
         const DeviceBase&     device,
         const Surface&        surface,
         SwapChain*            oldSwapchain = nullptr);

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
       * @brief Get the index of the next available image in the swap chain.
       * @details After calling this function, the user should check whether the swap chain needs to be recreated with the ad-hoc function.
       * @param semaphore Optional semaphore for the GPU to signal when the presentation engine has finished using the image.
       * @param fence Optional fence for the GPU to signal when the presentation engine has finished using the image.
       * @return Index of the next available image in the swap chain.
       */
      [[nodiscard]] u32 acquireNextImage(Semaphore* semaphore = nullptr, Fence* fence = nullptr);

      /**
       * @brief Get the current format of the swap chain.
       * @return Current format of the swap chain.
       */
      [[nodiscard]] const VkSurfaceFormatKHR& format() const;

      /**
       * @brief Get the raw handle to the swap chain.
       */
      [[nodiscard]] VkSwapchainKHR handle() const;

      /**
       * @brief Get the images of the swap chain.
       * @return Images of the swap chain.
       */
      [[nodiscard]] const std::vector<ImageView>& images() const;

      /**
       * @brief Check whether the swap chain needs to be recreated or not.
       * @return true if the swap chain needs to be recreated, false otherwise.
       */
      [[nodiscard]] bool needsRecreation() const;

   private:
      VkSwapchainKHR     swapChain_{ VK_NULL_HANDLE }; /**< Raw swap chain handle. */
      VkSurfaceFormatKHR format_;                      /**< Current format of the swap chain. */
      VkPresentModeKHR   mode_;                        /**< Current mode of the swap chain. */
      VkExtent2D         extent_;                      /**< Current extent of the swap chain. */

      const DeviceBase& device_; /**< Logical device this swap chain is bound to. */

      std::vector<ImageView> images_; /**< Swap chain images. */

      bool needsRecreation_{ false }; /**< If true, the swap chain needs to be recreated. */
   };
} // namespace rw::lif::vlk

#endif