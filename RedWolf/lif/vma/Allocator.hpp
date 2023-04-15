#ifndef RW_LIF_VMA_MANAGER_HPP
#define RW_LIF_VMA_MANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vma/Initialiser.hpp"

#include <variant>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vma
{
   class Interface;
}

namespace rw::utils
{
   class Logger;
}

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
       * @param manager RedWolf library manager.
       * @param physicalDevice Physical device the allocator will work on.
       * @param device Logical device the allocator will work on.
       */
      Allocator(RedWolfManager& manager, VkPhysicalDevice physicalDevice, VkDevice device);

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

      /**
       * @brief Create a Vulkan buffer and allocate memory for it.
       * @param bufferInfo Buffer creation parameters.
       * @param allocationInfo Memory allocation parameters.
       * @return Newly created buffer, memory allocation and allocation information, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo>
         createBuffer(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) const;

      /**
       * @brief Destroy a buffer created by this allocator.
       * @param buffer Buffer to destroy.
       * @param allocation Memory allocated to the buffer.
       */
      void destroyBuffer(VkBuffer buffer, VmaAllocation allocation) const;

      /**
       * @brief Map a memory allocation to a local pointer.
       * @return Pointer to the allocated memory, or nullptr in case of an error.
       */
      [[nodsicard]] void* mapMemory(VmaAllocation allocation) const;

      /**
       * @brief Unmap a previously mapped memory allocation.
       * @param allocation Memory allocation to unmap.
       */
      void unmapMemory(VmaAllocation allocation) const;

   private:
      VmaAllocator allocator_{ VK_NULL_HANDLE }; /**< Raw handle to the VMA allocator. */

      rw::utils::Logger& logger_;                           /**< Library logger. */
      VkInstance         instance_{ VK_NULL_HANDLE };       /**< Vulkan instance the allocator will work on. */
      VkPhysicalDevice   physicalDevice_{ VK_NULL_HANDLE }; /**< Physical device the allocator will work on. */
      VkDevice           device_{ VK_NULL_HANDLE };         /**< Logical device the allocator will work on. */
   };
} // namespace rw::lif::vma

#endif