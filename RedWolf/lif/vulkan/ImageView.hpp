#ifndef RW_LIF_VULKAN_IMAGEVIEW_HPP
#define RW_LIF_VULKAN_IMAGEVIEW_HPP

#include "RedWolf/lif/vulkan/BaseObject.hpp"

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   class GraphicsDevice;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan image view.
    */
   class RW_API ImageView : public BaseObject
   {
   public:
      /**
       * @brief Create an image view object.
       * @param manager RedWolf library manager.
       * @param graphicsDevice Logical device that will render to the image.
       * @param image Image the view is based on.
       * @param format Image format.
       */
      ImageView(RedWolfManager& manager, GraphicsDevice& graphicsDevice, VkImage image, VkFormat format);

      /**
       * @brief Destructor.
       */
      ~ImageView() override;

      /**
       * @brief Copy constructor.
       */
      ImageView(const ImageView&) = delete;

      /**
       * @brief Move constructor.
       */
      ImageView(ImageView&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      ImageView& operator=(const ImageView&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      ImageView& operator=(ImageView&&) = delete;

      /**
       * @brief Get the raw handle to the image view.
       * @return Raw handle to the image view.
       */
      [[nodiscard]] VkImageView handle();

   private:
      VkImageView view_{ VK_NULL_HANDLE }; /**< Raw handle to the image view. */

      VkDevice device_{ VK_NULL_HANDLE }; /**< Raw handle of the device the image view is bound to. */
      VkImage  image_{ VK_NULL_HANDLE };  /**< Raw image handle. */
   };
} // namespace rw::lif::vlk

#endif