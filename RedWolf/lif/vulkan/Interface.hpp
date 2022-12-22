#ifndef RW_LIF_VULKAN_INTERFACE_HPP
#define RW_LIF_VULKAN_INTERFACE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/FunctionPointers.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <format>
#include <map>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   /**
    * @brief Interface for all library calls.
    */
   class RW_API Interface
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      explicit Interface(RedWolfManager& manager);

      /**
       * @brief Destructor.
       */
      ~Interface() = default;

      /**
       * @brief Copy constructor.
       */
      Interface(const Interface&) = delete;

      /**
       * @brief Move constructor.
       */
      Interface(Interface&&) = default;

      /**
       * @brief Copy-assignment operator.
       */
      Interface& operator=(const Interface&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Interface& operator=(Interface&&) = delete;

      /**
       * @brief Allocate a new command buffer.
       * @param device Graphics device that will execute the command buffer.
       * @param commandBufferInfo Command buffer creation parameters.
       */
      [[nodiscard]] VkCommandBuffer allocateCommandBuffer(VkDevice device, const VkCommandBufferAllocateInfo& commandBufferInfo);

      /**
       * @brief Start recording a command buffer.
       * @param commandBuffer Command buffer to start recording.
       * @param beginInfo Command buffer recording parameters.
       * @return true on success, false otherwise.
       */
      bool beginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo& beginInfo);

      /**
       * @brief Check whether a queue family of a physical device supports presenting images to a given surface.
       * @param device Device that owns the queue family.
       * @param queueFamily Index of the queue family to check.
       * @param surface Surface to draw to.
       * @return true if the queue family supports drawing to the given surface, false otherwise.
       */
      [[nodiscard]] bool checkSurfaceSupport(VkPhysicalDevice device, uint32_t queueFamily, VkSurfaceKHR surface) const;

      /**
       * @brief Begin a render pass.
       * @param commandBuffer Command buffer to start the render pass on.
       * @param renderPassInfo Render pass parameters.
       */
      void cmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo& renderPassInfo);

      /**
       * @brief Bind a pipeline to a command buffer.
       * @param commandBuffer Command buffer to bind the pipeline to.
       * @param pipeline Pipeline to bind to the command buffer.
       * @param pipelineType Type of pipeline to bind.
       */
      void cmdBindPipeline(VkCommandBuffer commandBuffer, VkPipeline pipeline, VkPipelineBindPoint pipelineType);

      /**
       * @brief End the active render pass for a command buffer.
       * @param commandBuffer Command buffer that needs to end the render pass.
       */
      void cmdEndRenderPass(VkCommandBuffer commandBuffer);

      /**
       * @brief Create a command pool.
       * @param device Logical device that will execute the commands from the pool.
       * @param commandPoolInfo Command pool creation parameters.
       * @return Handle to the newly created command pool, or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkCommandPool createCommandPool(VkDevice device, const VkCommandPoolCreateInfo& commandPoolInfo);

      /**
       * @brief Create a debug utils messenger for an instance.
       * @param instance Target instance.
       * @param args Debug messenger arguments.
       * @return New debug messenger, if creation was successful.
       */
      [[nodiscard]] std::optional<VkDebugUtilsMessengerEXT>
         createDebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& args);

      /**
       * @brief Create a logical device from a physical device.
       * @param physicalDevice Physical Vulkan device.
       * @param deviceInfo Settings for the new device.
       * @return Handle to the new logical device. The caller gets the ownership.
       */
      [[nodiscard]] VkDevice createDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo& deviceInfo);

      /**
       * @brief Create a framebuffer.
       * @param device Logical device this framebuffer will be used on.
       * @param framebufferInfo Framebuffer creation parameters.
       * @return Handle to the newly created framebuffer, or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkFramebuffer createFramebuffer(VkDevice device, const VkFramebufferCreateInfo& framebufferInfo);

      /**
       * @brief Create a graphics pipeline.
       * @param device Logical device this pipeline will be used on.
       * @param pipelineInfo Pipeline creation parameters.
       * @return Handle to the newly created graphics pipeline, or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkPipeline createGraphicsPipeline(VkDevice device, const VkGraphicsPipelineCreateInfo& pipelineInfo);

      /**
       * @brief Create an image view.
       * @param device Logical device the image view is bound to.
       * @param viewInfo Creation parameters.
       * @return Handle to the created image view or VK_NULL_HANDLE in case of error.
       */
      [[nodiscard]] VkImageView createImageView(VkDevice device, const VkImageViewCreateInfo& viewInfo);

      /**
       * @brief Create a Vulkan instance.
       * @param args Instance arguments.
       * @return Newly created instance or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkInstance createInstance(const VkInstanceCreateInfo& args);

      /**
       * @brief Create a pipeline layout.
       * @param device Logical device that will use the pipeline.
       * @param pipelineInfo Pipeline creation parameters.
       * @return Newly created pipeline layout, or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkPipelineLayout createPipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo& pipelineInfo);

      /**
       * @brief Create a render pass.
       * @param device Device to which the render pass will be bound.
       * @param renderPassInfo Render pass creation parameters.
       * @return Newly created render pass, or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkRenderPass createRenderPass(VkDevice device, const VkRenderPassCreateInfo& renderPassInfo);

      /**
       * @brief Create a Vulkan shader module.
       * @param device Logical device that will use the shader.
       * @param shaderInfo Shader parameters.
       * @return Newly created shader module or VK_NULL_HANDLE>
       */
      [[nodiscard]] VkShaderModule createShaderModule(VkDevice device, const VkShaderModuleCreateInfo& shaderInfo);

      /**
       * @brief Create a swap chain.
       * @param device Logical device the swap chain will be bound to.
       * @param createInfo Swap chain creation parameters.
       * @return New swap chain, or VK_NULL_HANDLE if an error occured.
       */
      [[nodiscard]] VkSwapchainKHR createSwapChain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo);

      /**
       * @brief Destroy a command pool.
       * @param device Device the command pool is bound to.
       * @param commanPool Command pool to destroy.
       */
      void destroyCommandPool(VkDevice device, VkCommandPool commandPool);

      /**
       * @brief Destroy a debug utils messenger for an instance.
       * @param instance Target instance.
       * @param messenger Debug messenger to destroy.
       */
      void destroyDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT messenger);

      /**
       * @brief Destroy a physical device.
       * @param device Device to destroy.
       */
      void destroyDevice(VkDevice device);

      /**
       * @brief Destroy a framebuffer.
       * @param device Device the framebuffer is bound to.
       * @param framebuffer Framebuffer to destroy.
       */
      void destroyFramebuffer(VkDevice device, VkFramebuffer framebuffer);

      /**
       * @brief Destroy an image view.
       * @param device Device the image view is bound to.
       * @param imageView Image view to destroy.
       */
      void destroyImageView(VkDevice device, VkImageView imageView);

      /**
       * @brief Destroy an instance.
       * @param instance Instance to destroy.
       */
      void destroyInstance(VkInstance instance);

      /**
       * @brief Destroy a pipeline.
       * @param device Device the pipeline is bound to.
       * @param pipeline Pipeline to destroy.
       */
      void destroyPipeline(VkDevice device, VkPipeline pipeline);

      /**
       * @brief Destroy a pipeline layout.
       * @param device Logical device the pipeline layout is bound to.
       * @param pipelineLayout Pipeline layout to destroy.
       */
      void destroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout);

      /**
       * @brief Destroy a render pass.
       * @param device Logical device the render pass is bound to.
       * @param renderPass Render pass to destroy.
       */
      void destroyRenderPass(VkDevice device, VkRenderPass renderPass);

      /**
       * @brief Destroy a shader module.
       * @param device Logical device that uses the shader.
       * @param shader Shader to destroy.
       */
      void destroyShaderModule(VkDevice device, VkShaderModule shader);

      /**
       * @brief Destroy a window surface.
       * @param instance Instance the surface is bound to.
       * @param surface Surface to destroy.
       */
      void destroySurface(VkInstance instance, VkSurfaceKHR surface);

      /**
       * @brief Destroy a given swap chain.
       * @param device The logical device the swap chain is bound to.
       * @param swapChain The swap chain to destroy.
       */
      void destroySwapChain(VkDevice device, VkSwapchainKHR swapChain);

      /**
       * @brief Stop the recording of a command buffer.
       * @param commandBuffer Command buffer to stop recording.
       * @return true on success, false otherwise.
       */
      bool endCommandBuffer(VkCommandBuffer commandBuffer);

      /**
       * @brief Retrieve the properties of all extensions supported by a given physical device.
       * @param device Physical device to check extension properties of.
       * @return List of properties of all extensions supported by the device.
       */
      [[nodiscard]] std::vector<VkExtensionProperties> enumerateDeviceExtensionProperties(VkPhysicalDevice device);

      /**
       * @brief Retrieve the properties of all extensions supported by Vulkan instances.
       * @return List of properties of all extensions supported by Vulkan instances.
       */
      [[nodiscard]] std::vector<VkExtensionProperties> enumerateInstanceExtensionProperties();

      /**
       * @brief Get a specific queue for a given device.
       * @param device Device from which to get the queue.
       * @param familyIndex Index of the queue family.
       * @param queueIndex Index of the specific queue.
       * @return Required queue, if present.
       */
      [[nodiscard]] VkQueue getDeviceQueue(VkDevice device, uint32_t familyIndex, uint32_t queueIndex = 0U) const;

      /**
       * @brief Get data about a physical device.
       * @param device Device to get the data for.
       * @return Device properties and features.
       */
      [[nodiscard]] std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> getPhysicalDeviceData(VkPhysicalDevice device) const;

      /**
       * @brief Get the list of available physical devices for a given instance.
       * @param instance Instance to get the devices from.
       * @return List of available physical devices.
       */
      [[nodiscard]] std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance) const;

      /**
       * @brief Get the queue families supported by a specific device.
       * @return Queue families supported by the given device.
       */
      [[nodiscard]] std::vector<VkQueueFamilyProperties> getPhysicalDeviceQueueFamilies(VkPhysicalDevice device) const;

      /**
       * @brief Get the capabilities of a physical device and surface.
       * @param device The physical device to check.
       * @param surface The surface to check.
       * @return Surface capabilities with the given device.
       */
      [[nodiscard]] VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface) const;

      /**
       * @brief Get the image formats supported by a surface on a device.
       * @param device The device the surface is bound to.
       * @param surface The surface to check.
       * @return List of image formats supported by the surface on the device.
       */
      [[nodiscard]] std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) const;

      /**
       * @brief Get the presentation modes supported by a surface on a device.
       * @param device The device the surface is bound to.
       * @param surface The surface to check.
       * @return List of presentation modes supported by a surface on a device.
       */
      [[nodiscard]] std::vector<VkPresentModeKHR> getSurfacePresentationModes(VkPhysicalDevice device, VkSurfaceKHR surface) const;

      /**
       * @brief Get the list of image handles of a specific swap chain.
       * @param device The logical device the swap chain is bound to.
       * @param swapChain The target swap chain.
       * @return List of raw handles to the swap chain's images.
       */
      [[nodiscard]] std::vector<VkImage> getSwapChainImages(VkDevice device, VkSwapchainKHR swapChain) const;

   private:
      /**
       * @brief Call a Vulkan function with error logging only if in debug mode.
       * @tparam FunctionT Function type to call.
       * @tparam Args Types of arguments of the Vulkan function.
       * @param function Actual function to call.
       * @param args Actual Vulkan function arguments.
       */
      template<typename FunctionT, typename... Args>
      void callVulkanFunction_(FunctionT function, Args&&... args) const
      {
         if constexpr (rw::debug)
         {
            VkResult errorCode{ function(std::forward<Args>(args)...) };
            if (errorCode != VK_SUCCESS)
            {
               logger_.warn("Vulkan error: {}.", errorCode);
            }
         }
         else
         {
            function(std::forward<Args>(args)...);
         }
      }

      /**
       * @brief Initialise a single Vulkan function.
       * @tparam FunctionT Function pointer type.
       * @param instance Vulkan instance.
       * @param address Address to store the pointer.
       * @param functionName Name of the function to initialise.
       * @return true on success, false otherwise.
       */
      template<typename FunctionT>
      [[nodiscard]] bool getFunctionPointer_(VkInstance instance, FunctionT& address, std::string_view functionName)
      {
         address = reinterpret_cast<FunctionT>(vkGetInstanceProcAddr(instance, functionName.data()));
         return address != nullptr;
      }

      /**
       * @brief Initialise the function pointers for a given instance.
       * @param instance Instance to initialise the function pointers for.
       * @return true on success, false otherwise.
       */
      bool initialiseFunctions_(VkInstance instance);

      rw::utils::Logger& logger_; /**< Library logger. */

      std::map<VkInstance, FunctionPointers> functions_; /**< Instance-specific function pointers. */
   };
} // namespace rw::lif::vlk

/**
 * @brief std::formatter specialization for VkResult
 */
template<>
struct std::formatter<VkResult> : std::formatter<std::string>
{
   auto format(const VkResult& data, std::format_context& ctx)
   {
      switch (data)
      {
      case VkResult::VK_SUCCESS:
         return std::formatter<std::string>::format("VK_SUCCESS", ctx);
         break;
      case VkResult::VK_NOT_READY:
         return std::formatter<std::string>::format("VK_NOT_READY", ctx);
         break;
      case VkResult::VK_TIMEOUT:
         return std::formatter<std::string>::format("VK_TIMEOUT", ctx);
         break;
      case VkResult::VK_EVENT_SET:
         return std::formatter<std::string>::format("VK_EVENT_SET", ctx);
         break;
      case VkResult::VK_EVENT_RESET:
         return std::formatter<std::string>::format("VK_EVENT_RESET", ctx);
         break;
      case VkResult::VK_INCOMPLETE:
         return std::formatter<std::string>::format("VK_INCOMPLETE", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_HOST_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_DEVICE_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_INITIALIZATION_FAILED:
         return std::formatter<std::string>::format("VK_ERROR_INITIALIZATION_FAILED", ctx);
         break;
      case VkResult::VK_ERROR_DEVICE_LOST:
         return std::formatter<std::string>::format("VK_ERROR_DEVICE_LOST", ctx);
         break;
      case VkResult::VK_ERROR_MEMORY_MAP_FAILED:
         return std::formatter<std::string>::format("VK_ERROR_MEMORY_MAP_FAILED", ctx);
         break;
      case VkResult::VK_ERROR_LAYER_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_LAYER_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_EXTENSION_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_FEATURE_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_FEATURE_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER:
         return std::formatter<std::string>::format("VK_ERROR_INCOMPATIBLE_DRIVER", ctx);
         break;
      case VkResult::VK_ERROR_TOO_MANY_OBJECTS:
         return std::formatter<std::string>::format("VK_ERROR_TOO_MANY_OBJECTS", ctx);
         break;
      case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED:
         return std::formatter<std::string>::format("VK_ERROR_FORMAT_NOT_SUPPORTED", ctx);
         break;
      case VkResult::VK_ERROR_FRAGMENTED_POOL:
         return std::formatter<std::string>::format("VK_ERROR_FRAGMENTED_POOL", ctx);
         break;
      case VkResult::VK_ERROR_UNKNOWN:
         return std::formatter<std::string>::format("VK_ERROR_UNKNOWN", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_POOL_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_EXTERNAL_HANDLE", ctx);
         break;
      case VkResult::VK_ERROR_FRAGMENTATION:
         return std::formatter<std::string>::format("VK_ERROR_FRAGMENTATION", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS", ctx);
         break;
      case VkResult::VK_PIPELINE_COMPILE_REQUIRED:
         return std::formatter<std::string>::format("VK_PIPELINE_COMPILE_REQUIRED", ctx);
         break;
      case VkResult::VK_ERROR_SURFACE_LOST_KHR:
         return std::formatter<std::string>::format("VK_ERROR_SURFACE_LOST_KHR", ctx);
         break;
      case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
         return std::formatter<std::string>::format("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR", ctx);
         break;
      case VkResult::VK_SUBOPTIMAL_KHR:
         return std::formatter<std::string>::format("VK_SUBOPTIMAL_KHR", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_DATE_KHR", ctx);
         break;
      case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
         return std::formatter<std::string>::format("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR", ctx);
         break;
      case VkResult::VK_ERROR_VALIDATION_FAILED_EXT:
         return std::formatter<std::string>::format("VK_ERROR_VALIDATION_FAILED_EXT", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_SHADER_NV:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_SHADER_NV", ctx);
         break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
      case VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT", ctx);
         break;
      case VkResult::VK_ERROR_NOT_PERMITTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_NOT_PERMITTED_KHR", ctx);
         break;
      case VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
         return std::formatter<std::string>::format("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT", ctx);
         break;
      case VkResult::VK_THREAD_IDLE_KHR:
         return std::formatter<std::string>::format("VK_THREAD_IDLE_KHR", ctx);
         break;
      case VkResult::VK_THREAD_DONE_KHR:
         return std::formatter<std::string>::format("VK_THREAD_DONE_KHR", ctx);
         break;
      case VkResult::VK_OPERATION_DEFERRED_KHR:
         return std::formatter<std::string>::format("VK_OPERATION_DEFERRED_KHR", ctx);
         break;
      case VkResult::VK_OPERATION_NOT_DEFERRED_KHR:
         return std::formatter<std::string>::format("VK_OPERATION_NOT_DEFERRED_KHR", ctx);
         break;
      case VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
         return std::formatter<std::string>::format("VK_ERROR_COMPRESSION_EXHAUSTED_EXT", ctx);
         break;
      case VkResult::VK_RESULT_MAX_ENUM:
         return std::formatter<std::string>::format("VK_RESULT_MAX_ENUM", ctx);
         break;
      }

      return std::formatter<std::string>::format("UNDEFINED", ctx);
   }
};

#endif