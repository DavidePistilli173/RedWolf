#ifndef RW_LIF_VULKAN_QUEUE_HPP
#define RW_LIF_VULKAN_QUEUE_HPP

#include "RedWolf/common.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace rw::lif::vulkan
{
   /**
    * @brief Wrapper for a VkQueue.
    */
   class Queue
   {
   public:
      /**
       * @brief Queue operation types.
       */
      enum class OpType
      {
         graphics,    /**< Rendering. */
         presentation /**< Image presentation. */
      };

      Queue(VkQueue queue);

      /**
       * @brief Get the raw handle of the queue.
       * @return Raw handle of the queue.
       */
      [[nodiscard]] VkQueue handle() const;

      /**
       * @brief Check if a given operation type is supported by the queue.
       * @param operationType Type of operation to check support for.
       * @return true if the operation type is supported, false otherwise.
       */
      [[nodiscard]] bool isOperationSupported(OpType operationType) const;

      /**
       * @brief Get the list of operations supported by the queue.
       * @return List of operations supported by the queue.
       */
      [[nodiscard]] const std::vector<OpType>& supportedOperations() const;

   private:
      VkQueue queue_{ VK_NULL_HANDLE }; /**< Raw queue handle. */

      std::vector<OpType> supportedOperations_; /**< List of all operation types supported by the queue. */
   };
} // namespace rw::lif::vulkan

#endif