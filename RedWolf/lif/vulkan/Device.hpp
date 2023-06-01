#ifndef RW_LIF_VULKAN_DEVICE_HPP
#define RW_LIF_VULKAN_DEVICE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vma/Allocator.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/Queue.hpp"
#include "RedWolf/util/Logger.hpp"

#include <map>
#include <memory>

namespace rw::lif::vulkan
{
   /**
    * @brief Wrapper for a VkDevice.
    */
   class RW_API Device
   {
   public:
      /**
       * @brief Constructor.
       * @tparam QueueListT Type of list used to specify the required queues.
       * @tparam StringListT Type of list used to specify the required extensions.
       * @param logger Logger.
       * @param physicalDevice Physical device this device is bound to.
       * @param requiredQueues List of queues that are required for this device.
       * @param requiredExtensions List of extensions that are required for this device.
       */
      template<rw::cont::IsContiguousContainerOf<Queue::OpType> QueueListT, rw::cont::IsContiguousContainerOf<const char*> StringListT>
      Device(
         rw::util::Logger&     logger,
         const PhysicalDevice& physicalDevice,
         const QueueListT&     requiredQueues,
         const StringListT&    requiredExtensions) :
         logger_{ logger },
         physicalDevice_{ physicalDevice }
      {
         // Queues.
         const std::vector<VkQueueFamilyProperties>& queueProperties{ physicalDevice_.queueFamilyProperties() };
         std::vector<VkDeviceQueueCreateInfo>        queueCreateInfos{ initialiseQueueData_(queueProperties, requiredQueues) };

         // Device features.
         VkPhysicalDeviceFeatures deviceFeatures{};

         // Create the logical device.
         VkDeviceCreateInfo createInfo{};
         createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
         createInfo.pQueueCreateInfos = queueCreateInfos.data();
         createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
         createInfo.pEnabledFeatures = &deviceFeatures;
         createInfo.enabledExtensionCount = static_cast<u32>(requiredExtensions.size());
         createInfo.ppEnabledExtensionNames = requiredExtensions.data();

         VkResult errCode{ vkCreateDevice(device_, &deviceInfo, nullptr, &device_) };
         if (errCode != VK_SUCCESS)
         {
            logger_.relFatal("Failed to create logical device: {}", errCode);
         }

         for (size_t i{ 0U }; i < static_cast<size_t>(QueueFamilies::Id::num_elements); ++i)
         {
            if (availableQueueFamilies.queues[i].has_value())
            {
               vkGetDeviceQueue(device_, availableQueueFamilies.queues[i].value(), 0U, &queues_[i]);
            }
         }

         memoryAllocator_ = std::make_unique<rw::lif::vma::Allocator>(manager_, physicalDevice_.handle(), device_);
      }

   private:
      /**
       * @brief Initialise the queue data for device creation.
       * @tparam ListT Type of list used to specify the required queues.
       * @param queueProperties Queue family availability data for the device.
       * @param requiredQueues List of all queues required for this device.
       */
      template<rw::cont::IsContiguousContainerOf<Queue::OpType> ListT>
      [[nodiscard]] std::vector<VkDeviceQueueCreateInfo>
         initialiseQueueData_(const std::vector<VkQueueFamilyProperties>& queueProperties, const ListT& requiredQueues)
      {
         for (const auto& properties : queueProperties)
         {
            // properties
         }

         std::set<u32> queueFamilyIndices;

         for (auto queueId : requiredQueues)
         {
            if (!availableQueueFamilies.queues[static_cast<size_t>(queueId)].has_value())
            {
               logger_.relFatal("The physical device does not support all required queues.");
            }

            queueFamilyIndices.insert(availableQueueFamilies.queues[static_cast<size_t>(queueId)].value());
         }

         std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

         // Common parameters.
         VkDeviceQueueCreateInfo queueCreateInfo{};
         queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         const float queuePriority{ 1.0F };
         queueCreateInfo.pQueuePriorities = &queuePriority;
         queueCreateInfo.queueCount = 1U;

         // Add all required queue info.
         for (auto index : queueFamilyIndices)
         {
            queueCreateInfo.queueFamilyIndex = index;
            queueCreateInfos.emplace_back(queueCreateInfo);
         }

         return queueCreateInfos;
      }

      rw::util::Logger&     logger_;         /**< Logger. */
      const PhysicalDevice& physicalDevice_; /**< Physical device this logical device is created on. */

      std::vector<Queue>                       queues_;    /**< Active device queues. */
      std::map<Queue::OpType, size_t>          queueMap_;  /**< Map each operation type to a queue that supports it. */
      std::unique_ptr<rw::lif::vma::Allocator> allocator_; /**< Buffer memory allocator. */
   };
} // namespace rw::lif::vulkan

#endif