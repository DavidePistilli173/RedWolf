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
   class GraphicsDevice;
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
       * @param graphicsDevice Graphics device this frame buffer will belong to.
       * @param renderPass Render pass that this frame buffer will be compatible to.
       * @param imageView View on the image this frame buffer will reference.
       * @param surface Surface the image belongs to.
       */
      FrameBuffer(RedWolfManager& manager, GraphicsDevice& graphicsDevice, RenderPass& renderPass, ImageView& imageView, Surface& surface);

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

   private:
      VkFramebuffer framebuffer_; /**< Raw framebuffer handle. */

      VkDevice device_{ VK_NULL_HANDLE }; /**< Raw handle of the device the framebuffer is bound to. */
   };
} // namespace rw::lif::vlk

#endif