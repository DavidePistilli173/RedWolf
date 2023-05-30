#ifndef RW_LIF_VMA_ALLOCATOR_HPP
#define RW_LIF_VMA_ALLOCATOR_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/util/Logger.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace rw::lif::vma
{
   /**
    * @brief Wrapper for a VmaAllocator.
    */
   class RW_API Allocator
   {
   public:
      /**
       * @brief Constructor.
       * @param vulkanInstance Handle to the Vulkan instance to use.
       * @param physicalDevice Physical device the allocator will work on.
       * @param device Logical device the allocator will work on.
       * @param logger Logger.
       */
      Allocator(VkInstance vulkanInstance, VkPhysicalDevice physicalDevice, VkDevice device, rw::util::Logger& logger);

      /**
       * @brief Destructor.
       */
      ~Allocator();

      /**
       * @brief Copy constructor.
       */
      Allocator(const Allocator&) = delete;

      /**
       * @brief Move constructor.
       * @param other Object to move from.
       */
      Allocator(Allocator&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      Allocator& operator=(const Allocator&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Allocator& operator=(Allocator&&) = delete;

   private:
      VmaAllocator allocator_{ VK_NULL_HANDLE }; /**< Raw handle to the VMA allocator. */

      rw::util::Logger& logger_;                           /**< Logger. */
      VkInstance        instance_{ VK_NULL_HANDLE };       /**< Vulkan instance the allocator will work on. */
      VkPhysicalDevice  physicalDevice_{ VK_NULL_HANDLE }; /**< Physical device the allocator will work on. */
      VkDevice          device_{ VK_NULL_HANDLE };         /**< Logical device the allocator will work on. */
   };
} // namespace rw::lif::vma

#endif