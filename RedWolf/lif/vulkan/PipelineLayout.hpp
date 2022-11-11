#ifndef RW_LIF_VULKAN_PIPELINELAYOUT_HPP
#define RW_LIF_VULKAN_PIPELINELAYOUT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class GraphicsDevice;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkPipelineLayout object.
    */
   class RW_API PipelineLayout : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Graphics device that will use this pipeline layout.
       */
      PipelineLayout(RedWolfManager& manager, GraphicsDevice& device);

      /**
       * @brief Destructor.
       */
      ~PipelineLayout();

      /**
       * @brief Copy constructor.
       */
      PipelineLayout(const PipelineLayout&) = delete;

      /**
       * @brief Move constructor.
       */
      PipelineLayout(PipelineLayout&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      PipelineLayout& operator=(const PipelineLayout&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      PipelineLayout& operator=(PipelineLayout&&) = delete;

      /**
       * @brief Get the raw pipeline layout handle.
       * @return Raw pipeline layout handle.
       */
      [[nodiscard]] VkPipelineLayout handle();

   private:
      VkPipelineLayout pipelineLayout_{ VK_NULL_HANDLE }; /**< Raw handle to the pipeline layout. */

      GraphicsDevice& device_; /**< Graphics device this pipeline layout is bound to. */
   };
} // namespace rw::lif::vlk

#endif