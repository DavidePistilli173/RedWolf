#ifndef RW_LIBIF_VULKAN_VULKANSURFACE_HPP
#define RW_LIBIF_VULKAN_VULKANSURFACE_HPP

#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"

#include <mutex>
#include <vector>
#include <vulkan/vulkan.h>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class Instance;
}

namespace rw::utils
{
   class Logger;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a VkSurfaceKHR.
    */
   class RW_API Surface
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
      Surface(Surface&& other);

      /**
       * @brief Copy-assignment operator.
       */
      Surface& operator=(const Surface&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Surface& operator=(Surface&& other);

      /**
       * @brief Get the capabilities of the surface.
       * @return Capabilities of the surface.
       */
      VkSurfaceCapabilitiesKHR capabilities() const;

      /**
       * @brief Get the formats supported by the surface.
       * @return Formats supported by the surface.
       */
      std::vector<VkSurfaceFormatKHR> formats() const;

      /**
       * @brief Get the best format that the surface can support.
       * @return Best format that the surface can support.
       */
      VkSurfaceFormatKHR getBestFormat() const;

      /**
       * @brief Get the best presentation mode that the surface can support.
       * @return Best presentation mode that the surface can support.
       */
      VkPresentModeKHR getBestMode() const;

      /**
       * @brief Get the surface's raw handle.
       * @return Raw handle to the surface.
       */
      [[nodiscard]] VkSurfaceKHR handle();

      /**
       * @brief Initialise the surface.
       * @param physicalDevice Device that will render to the surface.
       */
      bool init(VkPhysicalDevice physicalDevice);

      /**
       * @brief Get the modes supported by the surface.
       * @return Modes supported by the surface.
       */
      std::vector<VkPresentModeKHR> modes() const;

   private:
      rw::utils::Logger&          logger_;          /**< Library logger. */
      rw::lif::glfw::GlfwManager& glfwManager_;     /**< GLFW library manager. */
      Interface&                  vulkanInterface_; /**< Vulkan API interface. */

      mutable std::mutex mtx_; /**< Surface mutex. */

      Instance&                       instance_;                  /**< Vulkan instance that owns this surface. */
      VkSurfaceKHR                    surface_{ VK_NULL_HANDLE }; /**< Raw handle to the surface. */
      VkSurfaceCapabilitiesKHR        capabilities_;              /**< Surface capabilities. */
      std::vector<VkSurfaceFormatKHR> formats_;                   /**< Supported image formats. */
      std::vector<VkPresentModeKHR>   modes_;                     /**< Present modes. */
   };
} // namespace rw::lif::vlk

#endif