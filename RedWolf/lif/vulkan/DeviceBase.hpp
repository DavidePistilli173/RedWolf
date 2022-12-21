#ifndef RW_LIF_VULKAN_DEVICEBASE_HPP
#define RW_LIF_VULKAN_DEVICEBASE_HPP

#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"
#include "RedWolf/lif/vulkan/concepts.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <set>
#include <vector>

namespace rw::lif::vlk
{
   /**
    * @brief Base class for Vulkan logical devices.
    */
   class RW_API DeviceBase : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @tparam QueueListT Type of list used to specify the required queues.
       * @tparam StringListT Type of list used to specify the required extensions.
       * @param manager RedWolf library manager.
       * @param physicalDevice Physical device this device is bound to.
       * @param requiredQueues List of queues that are required for this device.
       * @param requiredExtensions List of extensions that are required for this device.
       */
      template<IsQueueFamilyIdList QueueListT, rw::cont::IsConstCharStarList StringListT>
      DeviceBase(
         RedWolfManager& manager, PhysicalDevice& physicalDevice, const QueueListT& requiredQueues, const StringListT& requiredExtensions) :
         BaseObject(manager),
         physicalDevice_{ physicalDevice }
      {
         // Queues.
         QueueFamilies                        availableQueueFamilies{ physicalDevice_.availableQueueFamilies() };
         std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{ initialiseQueueData_(availableQueueFamilies, requiredQueues) };

         // Device features.
         VkPhysicalDeviceFeatures deviceFeatures{};

         // Create the logical device.
         VkDeviceCreateInfo createInfo{};
         createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
         createInfo.pQueueCreateInfos = queueCreateInfos.data();
         createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
         createInfo.pEnabledFeatures = &deviceFeatures;
         createInfo.enabledExtensionCount = requiredExtensions.size();
         createInfo.ppEnabledExtensionNames = requiredExtensions.data();

         device_ = vulkanInterface_.createDevice(physicalDevice_.handle(), createInfo);
         if (device_ == VK_NULL_HANDLE)
         {
            logger_.relFatal("Failed to create device.");
         }

         for (size_t i{ 0U }; i < static_cast<size_t>(QueueFamilies::Id::num_elements); ++i)
         {
            if (availableQueueFamilies.queues[i].has_value())
            {
               queues_[i] = vulkanInterface_.getDeviceQueue(device_, availableQueueFamilies.queues[i].value());
               if (queues_[i] == VK_NULL_HANDLE)
               {
                  logger_.relFatal("Failed to get queue handle for index {}.", i);
               }
            }
         }
      }

      /**
       * @brief Destructor.
       */
      ~DeviceBase() override;

      /**
       * @brief Copy constructor.
       */
      DeviceBase(const DeviceBase&) = delete;

      /**
       * @brief Move constructor.
       */
      DeviceBase(DeviceBase&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      DeviceBase& operator=(const DeviceBase&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      DeviceBase& operator=(DeviceBase&& other) = delete;

      /**
       * @brief Get the available queue families for the current device.
       * @return Data on the available queue families for the current device.
       */
      [[nodiscard]] QueueFamilies availableQueueFamilies() const;

      /**
       * @brief Get the device's raw handle.
       * @return Device's raw handle.
       */
      [[nodiscard]] VkDevice handle() const;

   protected:
      PhysicalDevice& physicalDevice_; /**< Physical device this device is bound to. */

      VkDevice device_{ VK_NULL_HANDLE }; /**< Raw device handle. */
      /**
       * @brief Queue handles for the device.
       */
      std::array<VkQueue, static_cast<size_t>(QueueFamilies::Id::num_elements)> queues_{ VK_NULL_HANDLE, VK_NULL_HANDLE };

   private:
      /**
       * @brief Initialise the queue data for device creation.
       * @tparam ListT Type of list used to specify the required queues.
       * @param availableQueueFamilies Queue family availability data for the device.
       * @param requiredQueues List of all queues required for this device.
       */
      template<IsQueueFamilyIdList ListT>
      [[nodiscard]] std::vector<VkDeviceQueueCreateInfo>
         initialiseQueueData_(const QueueFamilies& availableQueueFamilies, const ListT& requiredQueues)
      {
         std::set<uint32_t> queueFamilyIndices;

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
   };
} // namespace rw::lif::vlk

#endif