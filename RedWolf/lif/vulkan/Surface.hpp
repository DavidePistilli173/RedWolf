#ifndef RW_LIBIF_VULKAN_VULKANSURFACE_HPP
#define RW_LIBIF_VULKAN_VULKANSURFACE_HPP

#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"

#include <memory>
#include <shared_mutex>
#include <vector>
#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class Instance;
   class PhysicalDevice;
} // namespace rw::lif::vlk

namespace rw::utils
{
   class Logger;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a VkSurfaceKHR.
    */
   class RW_API Surface : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param window GLFW window handle to which the surface is bound.
       */
      Surface(RedWolfManager& manager, GLFWwindow* window);

      /**
       * @brief Destructor.
       */
      ~Surface();

      /**
       * @brief Copy constructor.
       */
      Surface(const Surface&) = delete;

      /**
       * @brief Move constructor.
       */
      Surface(Surface&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      Surface& operator=(const Surface&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Surface& operator=(Surface&&) = delete;

      /**
       * @brief Get the capabilities of the surface.
       * @return Capabilities of the surface.
       */
      [[nodiscard]] VkSurfaceCapabilitiesKHR capabilities() const;

      /**
       * @brief Get the formats supported by the surface.
       * @return Formats supported by the surface.
       */
      [[nodiscard]] std::vector<VkSurfaceFormatKHR> formats() const;

      /**
       * @brief Get the surface's raw handle.
       * @return Raw handle to the surface.
       */
      [[nodiscard]] VkSurfaceKHR handle();

      /**
       * @brief Get the modes supported by the surface.
       * @return Modes supported by the surface.
       */
      [[nodiscard]] std::vector<VkPresentModeKHR> modes() const;

      /**
       * @brief Get the currently selected extent for the surface.
       * @return Currently selected extend for the surface.
       */
      [[nodiscard]] VkExtent2D selectedExtent() const;

      /**
       * @brief Get the currently selected format for the surface.
       * @return Currently selected format for the surface.
       */
      [[nodiscard]] VkSurfaceFormatKHR selectedFormat() const;

      /**
       * @brief Get the currently selected presentation mode for the surface.
       * @return Currently selected presentation mode for the surface.
       */
      [[nodiscard]] VkPresentModeKHR selectedMode() const;

      /**
       * @brief Set the physical and logical devices that will render on the surface.
       * @param physicalDevice Physical device that will render to the surface.
       * @param graphicsDevice Logical device that will render to the surface.
       * @return true on success, false otherwise.
       */
      bool setDevices(PhysicalDevice& physicalDevice, GraphicsDevice& graphicsDevice);

   private:
      /**
       * @brief Initialise the physical and graphics devices.
       * @param physicalDevice Physical device that will render to the surface.
       * @return true on success, false otherwise.
       */
      bool initDevices_(PhysicalDevice& physicalDevice);

      /**
       * @brief Initialise the swap chain for this surface.
       * @param physicalDevice Physical device that will present images to the swap chain.
       * @param graphicsDevice Logical device that will present images to the swap chain.
       * @return true on success, false otherwise.
       */
      bool initSwapChain_(PhysicalDevice& physicalDevice, GraphicsDevice& graphicsDevice);

      rw::lif::glfw::GlfwManager& glfwManager_; /**< GLFW library manager. */

      mutable std::shared_mutex mtx_; /**< Surface mutex. */

      GLFWwindow*  window_{ nullptr };         /**< Window that owns this surface. */
      VkSurfaceKHR surface_{ VK_NULL_HANDLE }; /**< Raw handle to the surface. */

      VkSurfaceCapabilitiesKHR        capabilities_; /**< Surface capabilities. */
      std::vector<VkSurfaceFormatKHR> formats_;      /**< Supported image formats. */
      std::vector<VkPresentModeKHR>   modes_;        /**< Present modes. */

      VkSurfaceFormatKHR selectedFormat_; /**< Currently selected format. */
      VkPresentModeKHR   selectedMode_;   /**< Currently selected presentation mode. */
      VkExtent2D         selectedExtent_; /**< Currently selected surface resolution. */

      std::unique_ptr<SwapChain> swapChain_; /**< Swap chain for the surface. */
   };
} // namespace rw::lif::vlk

#endif