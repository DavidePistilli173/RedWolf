#ifndef RW_LIF_VMA_BUFFER_HPP
#define RW_LIF_VMA_BUFFER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/util/Logger.hpp"

#include <vk_mem_alloc.h>

namespace rw::lif::vma
{
   /**
    * @brief Wrapper for a VkBuffer and its associated memory allocation data.
    * @tparam T Data type stored in the buffer.
    */
   template<typename T>
   class Buffer
   {
   public:
      /**
       * @brief Construct an empty buffer with a given size.
       * @param logger Logger.
       * @param allocator Allocator the buffer will be created with.
       * @param bufferUsage Usage of the buffer.
       * @param memoryUsage Usage of the memory.
       * @param memoryFlags Memory properties.
       * @param bufferSize Number of elements to store in the buffer.
       */
      Buffer(
         rw::util::Logger&        logger,
         VmaAllocator             allocator,
         VkBufferUsageFlags       bufferUsage,
         VmaMemoryUsage           memoryUsage,
         VmaAllocationCreateFlags memoryFlags,
         size_t                   bufferSize) :
         logger_{ logger },
         allocator_{ allocator }, size_{ bufferSize }, bufferUsage_{ bufferUsage }, memoryUsage_{ memoryUsage }, memoryFlags_{ memoryFlags }
      {
         if (allocator_ == VK_NULL_HANDLE)
         {
            logger_.relFatal("Cannot create a buffer from a null allocator.");
         }

         VkBufferCreateInfo bufferInfo{};
         bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
         bufferInfo.size = static_cast<VkDeviceSize>(size_) * sizeof(T);
         bufferInfo.usage = bufferUsage_;
         bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

         VmaAllocationCreateInfo allocationInfo{};
         allocationInfo.usage = memoryUsage_;
         allocationInfo.flags = memoryFlags_;

         std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo> result{ VK_NULL_HANDLE, VK_NULL_HANDLE, VmaAllocationInfo{} };

         VkResult errCode{ vmaCreateBuffer(
            allocator_, &bufferInfo, &allocationInfo, &std::get<0>(result), &std::get<1>(result), &std::get<2>(result)) };
         if (errCode != VK_SUCCESS)
         {
            logger_.relFatal("Failed to create buffer: {}.", errCode);
         }

         buffer_ = std::get<0>(result);
         memory_ = std::get<1>(result);
         memoryInfo_ = std::get<2>(result);
      }

      /**
       * @brief Destructor.
       */
      ~Buffer()
      {
         unmap();
         if (buffer_ != VK_NULL_HANDLE) vmaDestroyBuffer(allocator_, buffer_, allocation_);
      }

      /**
       * @brief Copy constructor.
       */
      Buffer(const Buffer&) = delete;

      /**
       * @brief Move constructor.
       * @param other Buffer to move from.
       */
      Buffer(Buffer&& other) noexcept :
         logger_{ other.logger_ }, allocator_{ other.allocator_ }, buffer_{ other.buffer_ }, memory_{ other.memory_ },
         memoryInfo_{ other.memoryInfo_ }, ptr_{ other.ptr_ }, size_{ other.size_ }, bufferUsage_{ other.bufferUsage_ },
         memoryUsage_{ other.memoryUsage_ }, memoryFlags_{ other.memoryFlags_ }
      {
         other.allocator_ = VK_NULL_HANDLE;
         other.buffer_ = VK_NULL_HANDLE;
         other.memory_ = VK_NULL_HANDLE;
         other.ptr_ = nullptr;
         other.size_ = 0U;
      }

      /**
       * @brief Copy-assignment operator.
       */
      Buffer& operator=(const Buffer&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Buffer& operator=(Buffer&&) = delete;

      /**
       * @brief Copy data from another buffer using the GPU.
       * @param cmdBuffer Command buffer to submit the copy command to.
       * @param buffer Buffer to copy from.
       */
      void copyFrom(CommandBuffer& cmdBuffer, const Buffer<T>& buffer)
      {
         if (buffer.size_ > size_)
         {
            logger_.warn("Source buffer too large, discarding excess data.");
         }

         VkBufferCopy copyInfo{};
         copyInfo.srcOffset = buffer.memoryInfo_.offset;
         copyInfo.dstOffset = memoryInfo_.offset;
         copyInfo.size = std::min(buffer.size_, size_) * sizeof(T);
         cmdBuffer.copyBuffer(buffer_, buffer.buffer_, copyInfo);
      }

      /**
       * @brief Get the buffer raw handle.
       * @return Raw buffer handle.
       */
      [[nodiscard]] VkBuffer handle() const
      {
         return buffer_;
      }

      /**
       * @brief Map the buffer memory.
       * @return true on success, false otherwise.
       */
      bool map()
      {
         if (ptr_ == nullptr)
         {
            VkResult errCode{ vmaMapMemory(allocator_, allocation, &ptr_) };
            if (errCode != VK_SUCCESS)
            {
               logger_.relErr("Failed to map buffer memory: {}.", errCode);
            }
         }
         return ptr_ != nullptr;
      }

      /**
       * @brief Check if the buffer is mapped or not.
       * @return true if the buffer is mapped, false otherwise.
       */
      [[nodiscard]] bool mapped() const
      {
         return ptr_ != nullptr;
      }

      /**
       * @brief Set new data for the buffer.
       * @details This method can only be called if the buffer is mappable and already mapped, otherwise it will not succeed.
       */
      template<rw::cont::IsContiguousContainerOf<T> ListT>
      void setData(const ListT& data)
      {
         // If the buffer is mapped.
         if (ptr_ != nullptr)
         {
            // If the size is greater than the current size, print a warning.
            if (data.size() > size_)
            {
               logger_.warn("Data size greater than buffer size, discarding excess data.");
            }
            memcpy(ptr_, data.data(), std::min(data.size(), size_) * sizeof(T));
         }
      }

      /**
       * @brief Get the size of the buffer.
       * @return Current size of the buffer.
       */
      [[nodiscard]] size_t size() const
      {
         return size_;
      }

      /**
       * @brief Unmap the buffer.
       */
      void unmap()
      {
         if (ptr_ != nullptr)
         {
            vmaUnmapMemory(allocator_, allocation);
            ptr_ = nullptr;
         }
      }

   private:
      rw::util::Logger& logger_; /**< Logger. */

      VmaAllocator allocator_{ VK_NULL_HANDLE }; /**< Allocator that owns the buffer's memory. */

      VkBuffer          buffer_{ VK_NULL_HANDLE }; /**< Raw handle to the buffer. */
      VmaAllocation     memory_{ VK_NULL_HANDLE }; /**< Raw handle to the memory that contains the buffer. */
      VmaAllocationInfo memoryInfo_{};             /**< Memory allocation information. */
      void*             ptr_{ nullptr };           /**< Pointer to the memory, if it was mapped. */

      size_t                   size_{ 0U };                           /**< Total buffer size in bytes. */
      VkBufferUsageFlags       bufferUsage_{ 0U };                    /**< Buffer usage. */
      VmaMemoryUsage           memoryUsage_{ VMA_MEMORY_USAGE_AUTO }; /**< Memory usage. */
      VmaAllocationCreateFlags memoryFlags_{ 0U };                    /**< Memory properties. */
   };
} // namespace rw::lif::vma

#endif