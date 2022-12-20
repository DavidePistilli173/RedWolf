#include "Interface.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

Interface::Interface(RedWolfManager& manager) : logger_{ manager.logger() } {}

bool Interface::checkSurfaceSupport(VkPhysicalDevice device, uint32_t queueFamily, VkSurfaceKHR surface) const
{
   VkBool32 presentSupport{ VK_FALSE };

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      callVulkanFunction_(vkGetPhysicalDeviceSurfaceSupportKHR, device, queueFamily, surface, &presentSupport);
   }

   return static_cast<bool>(presentSupport);
}

std::optional<VkDebugUtilsMessengerEXT>
   Interface::createDebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& args)
{
   std::optional<VkDebugUtilsMessengerEXT> result;
   VkDebugUtilsMessengerEXT                debugMessenger{ VK_NULL_HANDLE };

   if (functions_[instance].vkCreateDebugUtilsMessengerEXT(instance, &args, nullptr, &debugMessenger) == VK_SUCCESS)
   {
      result = debugMessenger;
   }

   return result;
}

VkDevice Interface::createDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo& deviceInfo)
{
   if (physicalDevice == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create a logical device from a null physical device.");
      return VK_NULL_HANDLE;
   }

   VkDevice result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateDevice, physicalDevice, &deviceInfo, nullptr, &result);

   return result;
}

VkFramebuffer Interface::createFramebuffer(VkDevice device, const VkFramebufferCreateInfo& framebufferInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create a framebuffer from a null logical device.");
      return VK_NULL_HANDLE;
   }

   VkFramebuffer result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateFramebuffer, device, &framebufferInfo, nullptr, &result);

   return result;
}

VkPipeline Interface::createGraphicsPipeline(VkDevice device, const VkGraphicsPipelineCreateInfo& pipelineInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create graphics pipeline from a null logical device.");
      return VK_NULL_HANDLE;
   }

   VkPipeline result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateGraphicsPipelines, device, VK_NULL_HANDLE, 1U, &pipelineInfo, nullptr, &result);

   return result;
}

VkImageView Interface::createImageView(VkDevice device, const VkImageViewCreateInfo& viewInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create an image view from a null logical device.");
      return VK_NULL_HANDLE;
   }

   VkImageView result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateImageView, device, &viewInfo, nullptr, &result);

   return result;
}

VkInstance Interface::createInstance(const VkInstanceCreateInfo& info)
{
   VkInstance     result{ VK_NULL_HANDLE };
   const VkResult errorCode{ vkCreateInstance(&info, nullptr, &result) };

   if (errorCode == VK_SUCCESS)
   {
      if (!initialiseFunctions_(result))
      {
         logger_.relErr("Failed to initialise instance function pointers.");
         destroyInstance(result);
      }
   }
   else
   {
      logger_.relErr("Failed to create the instance, error code {}.", errorCode);
   }

   return result;
}

VkPipelineLayout Interface::createPipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo& pipelineInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create pipeline layout from a null device.");
      return VK_NULL_HANDLE;
   }

   VkPipelineLayout result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreatePipelineLayout, device, &pipelineInfo, nullptr, &result);

   return result;
}

VkRenderPass Interface::createRenderPass(VkDevice device, const VkRenderPassCreateInfo& renderPassInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create render pass from a null device.");
      return VK_NULL_HANDLE;
   }

   VkRenderPass result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateRenderPass, device, &renderPassInfo, nullptr, &result);

   return result;
}

VkShaderModule Interface::createShaderModule(VkDevice device, const VkShaderModuleCreateInfo& shaderInfo)
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Cannot create shader module from a null device.");
      return VK_NULL_HANDLE;
   }

   VkShaderModule result{ VK_NULL_HANDLE };

   callVulkanFunction_(vkCreateShaderModule, device, &shaderInfo, nullptr, &result);

   return result;
}

VkSwapchainKHR Interface::createSwapChain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo)
{
   VkSwapchainKHR result{ VK_NULL_HANDLE };

   if (device != VK_NULL_HANDLE)
   {
      callVulkanFunction_(vkCreateSwapchainKHR, device, &createInfo, nullptr, &result);
   }

   return result;
}

void Interface::destroyDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT messenger)
{
   functions_[instance].vkDestroyDebugUtilsMessengerEXT(instance, messenger, nullptr);
}

void Interface::destroyDevice(VkDevice device)
{
   vkDestroyDevice(device, nullptr);
   logger_.trace("Destroyed logical device {}.", device);
}

void Interface::destroyFramebuffer(VkDevice device, VkFramebuffer framebuffer)
{
   vkDestroyFramebuffer(device, framebuffer, nullptr);
   logger_.trace("Destroyed framebuffer {}.", framebuffer);
}

void Interface::destroyImageView(VkDevice device, VkImageView imageView)
{
   vkDestroyImageView(device, imageView, nullptr);
   logger_.trace("Destroyed image view {}.", imageView);
}

void Interface::destroyInstance(VkInstance instance)
{
   functions_.erase(instance);
   vkDestroyInstance(instance, nullptr);
   logger_.trace("Destroyed instance {}.", instance);
}

void Interface::destroyPipeline(VkDevice device, VkPipeline pipeline)
{
   vkDestroyPipeline(device, pipeline, nullptr);
   logger_.trace("Destroyed pipeline {}.", pipeline);
}

void Interface::destroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout)
{
   vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
   logger_.trace("Destroyed pipeline layout {}.", pipelineLayout);
}

void Interface::destroyRenderPass(VkDevice device, VkRenderPass renderPass)
{
   vkDestroyRenderPass(device, renderPass, nullptr);
   logger_.trace("Destroyed render pass {}.", renderPass);
}

void Interface::destroyShaderModule(VkDevice device, VkShaderModule shader)
{
   vkDestroyShaderModule(device, shader, nullptr);
   logger_.trace("Destroyed shader module {}.", shader);
}

void Interface::destroySurface(VkInstance instance, VkSurfaceKHR surface)
{
   vkDestroySurfaceKHR(instance, surface, nullptr);
   logger_.trace("Destroyed surface {}.", surface);
}

void Interface::destroySwapChain(VkDevice device, VkSwapchainKHR swapChain)
{
   vkDestroySwapchainKHR(device, swapChain, nullptr);
   logger_.trace("Destroyed swap chain {}.", swapChain);
}

std::vector<VkExtensionProperties> Interface::enumerateDeviceExtensionProperties(VkPhysicalDevice device)
{
   std::vector<VkExtensionProperties> result;

   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Null device handle.");
      return result;
   }

   uint32_t count{ 0U };
   callVulkanFunction_(vkEnumerateDeviceExtensionProperties, device, nullptr, &count, nullptr);

   if (count > 0U)
   {
      result.resize(count);
      callVulkanFunction_(vkEnumerateDeviceExtensionProperties, device, nullptr, &count, result.data());
   }

   return result;
}

std::vector<VkExtensionProperties> Interface::enumerateInstanceExtensionProperties()
{
   std::vector<VkExtensionProperties> result;

   uint32_t count{ 0U };
   callVulkanFunction_(vkEnumerateInstanceExtensionProperties, nullptr, &count, nullptr);

   if (count > 0U)
   {
      result.resize(count);
      callVulkanFunction_(vkEnumerateInstanceExtensionProperties, nullptr, &count, result.data());
   }

   return result;
}

VkQueue Interface::getDeviceQueue(VkDevice device, uint32_t familyIndex, uint32_t queueIndex) const
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Null device handle.");
      return VK_NULL_HANDLE;
   }

   VkQueue result{ VK_NULL_HANDLE };
   vkGetDeviceQueue(device, familyIndex, queueIndex, &result);
   return result;
}

std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> Interface::getPhysicalDeviceData(VkPhysicalDevice device) const
{
   std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> result;

   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Null device handle.");
      return result;
   }

   if (device != VK_NULL_HANDLE)
   {
      vkGetPhysicalDeviceProperties(device, &result.first);
      vkGetPhysicalDeviceFeatures(device, &result.second);
   }

   return result;
}

std::vector<VkPhysicalDevice> Interface::getPhysicalDevices(VkInstance instance) const
{
   std::vector<VkPhysicalDevice> result;

   if (instance != VK_NULL_HANDLE)
   {
      uint32_t deviceCount{ 0U };
      callVulkanFunction_(vkEnumeratePhysicalDevices, instance, &deviceCount, nullptr);

      result.resize(deviceCount);
      callVulkanFunction_(vkEnumeratePhysicalDevices, instance, &deviceCount, result.data());
   }

   return result;
}

std::vector<VkQueueFamilyProperties> Interface::getPhysicalDeviceQueueFamilies(VkPhysicalDevice device) const
{
   std::vector<VkQueueFamilyProperties> result;

   if (device != VK_NULL_HANDLE)
   {
      uint32_t queueFamilyCount{ 0U };
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      result.resize(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, result.data());
   }

   return result;
}

VkSurfaceCapabilitiesKHR Interface::getSurfaceCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
   VkSurfaceCapabilitiesKHR result{};

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      callVulkanFunction_(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, device, surface, &result);
   }

   return result;
}

std::vector<VkSurfaceFormatKHR> Interface::getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
   std::vector<VkSurfaceFormatKHR> result;

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      uint32_t formatCount{ 0U };
      callVulkanFunction_(vkGetPhysicalDeviceSurfaceFormatsKHR, device, surface, &formatCount, nullptr);

      if (formatCount > 0U)
      {
         result.resize(formatCount);
         callVulkanFunction_(vkGetPhysicalDeviceSurfaceFormatsKHR, device, surface, &formatCount, result.data());
      }
   }

   return result;
}

std::vector<VkPresentModeKHR> Interface::getSurfacePresentationModes(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
   std::vector<VkPresentModeKHR> result;

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      uint32_t modeCount{ 0U };
      callVulkanFunction_(vkGetPhysicalDeviceSurfacePresentModesKHR, device, surface, &modeCount, nullptr);

      if (modeCount > 0U)
      {
         result.resize(modeCount);
         callVulkanFunction_(vkGetPhysicalDeviceSurfacePresentModesKHR, device, surface, &modeCount, result.data());
      }
   }

   return result;
}

std::vector<VkImage> Interface::getSwapChainImages(VkDevice device, VkSwapchainKHR swapChain) const
{
   std::vector<VkImage> result;

   if (device != VK_NULL_HANDLE && swapChain != VK_NULL_HANDLE)
   {
      uint32_t imageCount{ 0U };
      callVulkanFunction_(vkGetSwapchainImagesKHR, device, swapChain, &imageCount, nullptr);

      if (imageCount > 0U)
      {
         result.resize(imageCount);
         callVulkanFunction_(vkGetSwapchainImagesKHR, device, swapChain, &imageCount, result.data());
      }
   }

   return result;
}

bool Interface::initialiseFunctions_(VkInstance instance)
{
   auto [iterator, success] = functions_.emplace(std::make_pair(instance, FunctionPointers()));
   if (!success)
   {
      logger_.relErr("Failed to create map node.");
      return false;
   }

   bool result{ true };
   result = result && getFunctionPointer_(instance, iterator->second.vkCreateDebugUtilsMessengerEXT, "vkCreateDebugUtilsMessengerEXT");
   result = result && getFunctionPointer_(instance, iterator->second.vkDestroyDebugUtilsMessengerEXT, "vkDestroyDebugUtilsMessengerEXT");

   return result;
}