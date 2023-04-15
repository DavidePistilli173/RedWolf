#ifndef RW_LIF_VULKAN_FRAMEBUFFER_HPP
#define RW_LIF_VULKAN_FRAMEBUFFER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   class DeviceBase;
   class ImageView;
   class RenderPass;
   class Surface;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkFramebuffer.
    */
   class RW_API FrameBuffer : public BaseObject
   {
   public:
      /**
       * @brief Construct a Vulkan framebuffer.
       * @param manager RedWolf library manager.
       * @param device Logical device this frame buffer will belong to.
       * @param renderPass Render pass that this frame buffer will be compatible to.
       * @param imageView View on the image this frame buffer will reference.
       * @param surface Surface the image belongs to.
       */
      FrameBuffer(
         RedWolfManager&   manager,
         const DeviceBase& device,
         const RenderPass& renderPass,
         const ImageView&  imageView,
         const Surface&    surface);

      /**
       * @brief Destructor.
       */
      ~FrameBuffer() override;

      /**
       * @brief Copy constructor.
       */
      FrameBuffer(const FrameBuffer&) = delete;

      /**
       * @brief Move constructor.
       * @param other FrameBuffer to move from.
       */
      FrameBuffer(FrameBuffer&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      FrameBuffer& operator=(const FrameBuffer&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      FrameBuffer& operator=(FrameBuffer&&) = delete;

      /**
       * @brief Get the extent of the frame buffer.
       * @return Extent of the frame buffer.
       */
      [[nodiscard]] VkExtent2D extent() const;

      /**
       * @brief Get the raw handle to the frame buffer.
       * @return Raw handle to the frame buffer.
       */
      [[nodiscard]] VkFramebuffer handle() const;

   private:
      VkFramebuffer framebuffer_; /**< Raw framebuffer handle. */

      const DeviceBase& device_;         /**< Logical device the framebuffer is bound to. */
      VkExtent2D        extent_{ 0, 0 }; /**< Extent of the frame buffer. */
   };
} // namespace rw::lif::vlk

#endif