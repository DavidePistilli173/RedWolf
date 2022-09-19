#ifndef RW_LIF_VULKAN_INTERFACE_HPP
#define RW_LIF_VULKAN_INTERFACE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/FunctionPointers.hpp"

#include <map>
#include <optional>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
}

namespace rw::utils
{
   class Logger;
}

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
      Interface(RedWolfManager& manager);

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
      Interface& operator=(Interface&&) = default;

      /**
       * @brief Check whether a queue family of a physical device supports presenting images to a given surface.
       * @param device Device that owns the queue family.
       * @param queueFamily Index of the queue family to check.
       * @param surface Surface to draw to.
       * @return true if the queue family supports drawing to the given surface, false otherwise.
       */
      [[nodiscard]] bool checkSurfaceSupport(VkPhysicalDevice device, uint32_t queueFamily, VkSurfaceKHR surface) const;

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
       * @brief Create a Vulkan instance.
       * @param args Instance arguments.
       * @return Newly created instance or VK_NULL_HANDLE.
       */
      [[nodiscard]] VkInstance createInstance(const VkInstanceCreateInfo& args);

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
       * @brief Destroy an instance.
       * @param instance Instance to destroy.
       */
      void destroyInstance(VkInstance instance);

      /**
       * @brief Destroy a window surface.
       * @param instance Instance the surface is bound to.
       * @param surface Surface to destroy.
       */
      void destroySurface(VkInstance instance, VkSurfaceKHR surface);

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

   private:
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

#endif