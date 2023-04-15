#ifndef RW_LIF_VULKAN_DEVICEBASE_HPP
#define RW_LIF_VULKAN_DEVICEBASE_HPP

#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vma/Allocator.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"
#include "RedWolf/lif/vulkan/concepts.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <memory>
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
      template<IsQueueFamilyIdArray QueueListT, rw::cont::IsContiguousContainerOf<const char*> StringListT>
      DeviceBase(
         RedWolfManager&       manager,
         const PhysicalDevice& physicalDevice,
         const QueueListT&     requiredQueues,
         const StringListT&    requiredExtensions) :
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
         createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
         createInfo.pEnabledFeatures = &deviceFeatures;
         createInfo.enabledExtensionCount = static_cast<u32>(requiredExtensions.size());
         createInfo.ppEnabledExtensionNames = requiredExtensions.data();

         device_ = physicalDevice_.createDevice(createInfo);
         if (device_ == VK_NULL_HANDLE)
         {
            logger_.relFatal("Failed to create device.");
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
       * @brief Create a buffer and allocate memory for it.
       * @param bufferInfo Buffer creation parameters.
       * @param allocationInfo Memory allocation parameters.
       * @return Newly created buffer and its allocated memory, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] std::tuple<VkBuffer, VmaAllocation, VmaAllocationInfo>
         createBuffer(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) const;

      /**
       * @brief Create a command pool.
       * @param commandPoolInfo Command pool creation parameters.
       * @return Newly created command pool, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkCommandPool createCommandPool(const VkCommandPoolCreateInfo& commandPoolInfo) const;

      /**
       * @brief Create a fence.
       * @param fenceInfo Fence creation parameters.
       * @return Newly created fence or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkFence createFence(const VkFenceCreateInfo& fenceInfo) const;

      /**
       * @brief Create a framebuffer.
       * @param framebufferInfo Framebuffer creation parameters.
       * @return Handle to the newly created framebuffer, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkFramebuffer createFramebuffer(const VkFramebufferCreateInfo& framebufferInfo) const;

      /**
       * @brief Create an image view.
       * @param viewInfo Creation parameters.
       * @return Handle to the created image view or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkImageView createImageView(const VkImageViewCreateInfo& viewInfo) const;

      /**
       * @brief Create a pipeline layout.
       * @param pipelineLayoutInfo Pipeline creation parameters.
       * @return Newly created pipeline layout, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkPipelineLayout createPipelineLayout(const VkPipelineLayoutCreateInfo& pipelineLayoutInfo) const;

      /**
       * @brief Create a render pass.
       * @param renderPassInfo Render pass creation parameters.
       * @return Newly created render pass, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkRenderPass createRenderPass(const VkRenderPassCreateInfo& renderPassInfo) const;

      /**
       * @brief Create a semaphore.
       * @param semaphoreInfo Semaphore creation parameters.
       * @return Newly created semaphore, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkSemaphore createSemaphore(const VkSemaphoreCreateInfo& semaphoreInfo) const;

      /**
       * @brief Create a Vulkan shader module.
       * @param shaderInfo Shader parameters.
       * @return Newly created shader module, or VK_NULL_HANDLE in case of an error.
       */
      [[nodiscard]] VkShaderModule createShaderModule(const VkShaderModuleCreateInfo& shaderInfo) const;

      /**
       * @brief Create a swap chain.
       * @param swapchainInfo Swap chain creation parameters.
       * @return New swap chain, or an error code.
       */
      [[nodiscard]] VkSwapchainKHR createSwapchain(const VkSwapchainCreateInfoKHR& swapchainInfo) const;

      /**
       * @brief Destroy a buffer created by this device.
       * @param buffer Buffer to destroy.
       * @param allocation Memory allocation bound to the buffer.
       */
      void destroyBuffer(VkBuffer buffer, VmaAllocation allocation) const;

      /**
       * @brief Destroy a command pool created on this device.
       * @param commanPool Command pool to destroy.
       */
      void destroyCommandPool(VkCommandPool commandPool) const;

      /**
       * @brief Destroy a fence created on this device.
       * @param fence Fence to destroy.
       */
      void destroyFence(VkFence fence) const;

      /**
       * @brief Destroy a framebuffer created on this device.
       * @param framebuffer Framebuffer to destroy.
       */
      void destroyFramebuffer(VkFramebuffer framebuffer) const;

      /**
       * @brief Destroy an image view created on this device.
       * @param imageView Image view to destroy.
       */
      void destroyImageView(VkImageView imageView) const;

      /**
       * @brief Destroy a pipeline crated on this device.
       * @param pipeline Pipeline to destroy.
       */
      void destroyPipeline(VkPipeline pipeline) const;

      /**
       * @brief Destroy a pipeline layout created on this device.
       * @param pipelineLayout Pipeline layout to destroy.
       */
      void destroyPipelineLayout(VkPipelineLayout pipelineLayout) const;

      /**
       * @brief Destroy a fence created on this device.
       * @param fence Fence to destroy.
       */
      void destroyRenderPass(VkRenderPass renderPass) const;

      /**
       * @brief Destroy a semaphore created on this device.
       * @param semaphore Semaphore to destroy.
       */
      void destroySemaphore(VkSemaphore semaphore) const;

      /**
       * @brief Destroy a shader module created on this device.
       * @param shaderModule Shader module to destroy.
       */
      void destroyShaderModule(VkShaderModule shaderModule) const;

      /**
       * @brief Destroy a swap chain created on this device.
       * @param swapchain Swap chain to destroy.
       */
      void destroySwapchain(VkSwapchainKHR swapchain) const;

      /**
       * @brief Get the device's raw handle.
       * @return Device's raw handle.
       */
      [[nodiscard]] VkDevice handle() const;

      /**
       * @brief Map a memory allocation to a local pointer.
       * @return Pointer to the allocated memory, or an error code.
       */
      [[nodiscard]] void* mapMemory(VmaAllocation allocation) const;

      /**
       * @brief Get the queue with a given index, if present..
       * @param queueId Id of the requested queue.
       * @return Requested queue if present, otherwise VK_NULL_HANDLE;
       */
      [[nodiscard]] VkQueue queue(QueueFamilies::Id queueId) const;

      /**
       * @brief Unmap a previously mapped memory allocation.
       * @param allocation Memory allocation to unmap.
       */
      void unmapMemory(VmaAllocation allocation) const;

      /**
       * @brief Wait for the device to become idle. This is a blocking call.
       */
      void waitIdle();

   protected:
      const PhysicalDevice& physicalDevice_; /**< Physical device this device is bound to. */

      VkDevice device_{ VK_NULL_HANDLE }; /**< Raw device handle. */

      /**
       * @brief Queue handles for the device.
       */
      std::array<VkQueue, static_cast<size_t>(QueueFamilies::Id::num_elements)> queues_{ VK_NULL_HANDLE, VK_NULL_HANDLE };

      std::unique_ptr<rw::lif::vma::Allocator> memoryAllocator_; /**< Device memory allocator. */

   private:
      /**
       * @brief Initialise the queue data for device creation.
       * @tparam ListT Type of list used to specify the required queues.
       * @param availableQueueFamilies Queue family availability data for the device.
       * @param requiredQueues List of all queues required for this device.
       */
      template<IsQueueFamilyIdArray ListT>
      [[nodiscard]] std::vector<VkDeviceQueueCreateInfo>
         initialiseQueueData_(const QueueFamilies& availableQueueFamilies, const ListT& requiredQueues)
      {
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
   };
} // namespace rw::lif::vlk

#endif