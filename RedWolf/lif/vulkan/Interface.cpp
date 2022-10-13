#include "Interface.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

Interface::Interface(RedWolfManager& manager) : logger_{ manager.logger() } {}

bool Interface::checkSurfaceSupport(VkPhysicalDevice device, uint32_t queueFamily, VkSurfaceKHR surface) const
{
   VkBool32 presentSupport{ VK_FALSE };

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamily, surface, &presentSupport);
   }

   return presentSupport;
}

std::optional<VkDebugUtilsMessengerEXT>
   Interface::createDebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& args)
{
   std::optional<VkDebugUtilsMessengerEXT> result;
   VkDebugUtilsMessengerEXT                debugMessenger;

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

   VkDevice result;

   if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &result) != VK_SUCCESS)
   {
      logger_.relErr("Failed to create logical device.");
   }

   return result;
}

VkInstance Interface::createInstance(const VkInstanceCreateInfo& info)
{
   VkInstance result{ VK_NULL_HANDLE };
   VkResult   errorCode{ vkCreateInstance(&info, nullptr, &result) };

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

VkSwapchainKHR Interface::createSwapChain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo)
{
   VkSwapchainKHR result{ VK_NULL_HANDLE };

   if (device != VK_NULL_HANDLE)
   {
      if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &result) != VK_SUCCESS)
      {
         logger_.relErr("Failed to create swap chain.");
      }
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
}

void Interface::destroyInstance(VkInstance instance)
{
   functions_.erase(instance);
   vkDestroyInstance(instance, nullptr);
}

void Interface::destroySurface(VkInstance instance, VkSurfaceKHR surface)
{
   vkDestroySurfaceKHR(instance, surface, nullptr);
}

void Interface::destroySwapChain(VkDevice device, VkSwapchainKHR swapChain)
{
   vkDestroySwapchainKHR(device, swapChain, nullptr);
}

VkQueue Interface::getDeviceQueue(VkDevice device, uint32_t familyIndex, uint32_t queueIndex) const
{
   if (device == VK_NULL_HANDLE)
   {
      logger_.relErr("Null device handle.");
      return VK_NULL_HANDLE;
   }

   VkQueue result;
   vkGetDeviceQueue(device, familyIndex, queueIndex, &result);
   return result;
}

std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> Interface::getPhysicalDeviceData(VkPhysicalDevice device) const
{
   std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> result;

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
   VkPhysicalDevice              dev;

   if (instance != VK_NULL_HANDLE)
   {
      uint32_t deviceCount;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      result.resize(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, &dev);
   }

   return result;
}

std::vector<VkQueueFamilyProperties> Interface::getPhysicalDeviceQueueFamilies(VkPhysicalDevice device) const
{
   std::vector<VkQueueFamilyProperties> result;

   if (device != VK_NULL_HANDLE)
   {
      uint32_t queueFamilyCount;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      result.resize(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, result.data());
   }

   return result;
}

VkSurfaceCapabilitiesKHR Interface::getSurfaceCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
   VkSurfaceCapabilitiesKHR result{ VkResult::VK_ERROR_UNKNOWN };

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &result);
   }

   return result;
}

std::vector<VkSurfaceFormatKHR> Interface::getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
   std::vector<VkSurfaceFormatKHR> result;

   if (device != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
   {
      uint32_t formatCount{ 0U };
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
      if (formatCount > 0U)
      {
         result.resize(formatCount);
         vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, result.data());
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
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);
      if (modeCount > 0U)
      {
         result.resize(modeCount);
         vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, result.data());
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
      vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
      if (imageCount > 0U)
      {
         result.resize(imageCount);
         vkGetSwapchainImagesKHR(device, swapChain, &imageCount, result.data());
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