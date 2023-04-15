#ifndef RW_LIF_VULKAN_BUFFER_HPP
#define RW_LIF_VULKAN_BUFFER_HPP

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/common.hpp"
#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/CommandBuffer.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a VkBuffer and its associated VkDeviceMemory.
    * @tparam T Type stored in the buffer.
    */
   template<typename T>
   class RW_API Buffer : public BaseObject
   {
   public:
      /**
       * @brief Construct an empty buffer, with default size.
       * @param manager RedWolf library manager.
       * @param device Device the buffer will be created on.
       * @param bufferUsage Usage of the buffer.
       * @param memoryUsage Usage of the memory.
       * @param memoryFlags Memory properties.
       * @param bufferSize Number of elements to store in the buffer.
       */
      Buffer(
         RedWolfManager&          manager,
         const GraphicsDevice&    device,
         VkBufferUsageFlags       bufferUsage,
         VmaMemoryUsage           memoryUsage,
         VmaAllocationCreateFlags memoryFlags,
         size_t                   bufferSize) :
         BaseObject(manager),
         device_{ device }, size_{ bufferSize }, bufferUsage_{ bufferUsage }, memoryUsage_{ memoryUsage }, memoryFlags_{ memoryFlags }
      {
         createBuffer_();
      }

      /**
       * @brief Destructor.
       */
      ~Buffer()
      {
         unmap();
         device_.destroyBuffer(buffer_, memory_);
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
         BaseObject(other.manager_), buffer_{ other.buffer_ }, memory_{ other.memory_ }, ptr_{ other.ptr_ }, size_{ other.size_ },
         bufferUsage_{ other.bufferUsage_ }, memoryUsage_{ other.memoryUsage_ }, memoryFlags_{ other.memoryFlags_ }, device_{
            other.device_
         }
      {
         other.buffer_ = VK_NULL_HANDLE;
         other.memory_ = VK_NULL_HANDLE;
         other.ptr_ = nullptr;
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
       * @brief Get the buffer handle.
       * @return Raw buffer handle.
       */
      [[nodiscard]] VkBuffer buffer() const
      {
         return buffer_;
      }

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
       * @brief Map the buffer memory.
       * @return true on success, false otherwise.
       */
      bool map()
      {
         if (ptr_ == nullptr)
         {
            ptr_ = device_.mapMemory(memory_);
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
            device_.unmapMemory(memory_);
            ptr_ = nullptr;
         }
      }

   private:
      /**
       * @brief Utility function to create a buffer and allocate its memory.
       */
      void createBuffer_()
      {
         VkBufferCreateInfo bufferInfo{};
         bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
         bufferInfo.size = static_cast<VkDeviceSize>(size_) * sizeof(T);
         bufferInfo.usage = bufferUsage_;
         bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

         VmaAllocationCreateInfo allocationInfo{};
         allocationInfo.usage = memoryUsage_;
         allocationInfo.flags = memoryFlags_;

         std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo> result{ device_.createBuffer(bufferInfo, allocationInfo) };
         buffer_ = std::get<0>(result);
         memory_ = std::get<1>(result);
         memoryInfo_ = std::get<2>(result);

         if (buffer_ == VK_NULL_HANDLE || memory_ == VK_NULL_HANDLE)
         {
            logger_.relFatal("Failed to create buffer.");
         }
      }

      VkBuffer          buffer_{ VK_NULL_HANDLE }; /**< Raw handle to the buffer. */
      VmaAllocation     memory_{ VK_NULL_HANDLE }; /**< Raw handle to the memory that contains the buffer. */
      VmaAllocationInfo memoryInfo_{};             /**< Memory allocation information. */
      void*             ptr_{ nullptr };           /**< Pointer to the memory, if it was mapped. */

      size_t                   size_{ 0U };                           /**< Total buffer size in bytes. */
      VkBufferUsageFlags       bufferUsage_{ 0U };                    /**< Buffer usage. */
      VmaMemoryUsage           memoryUsage_{ VMA_MEMORY_USAGE_AUTO }; /**< Memory usage. */
      VmaAllocationCreateFlags memoryFlags_{ 0U };                    /**< Memory properties. */

      const GraphicsDevice& device_; /**< Device the buffer resides on. */
   };
} // namespace rw::lif::vlk

#endif