#include "Surface.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"

using namespace rw::lif::vlk;

Surface::Surface(RedWolfManager& manager, GLFWwindow* window) :
   logger_{ manager.logger() }, glfwManager_{ manager.glfwManager() }, vulkanInterface_{ manager.vulkanInterface() }, instance_{
      manager.vulkanInstance()
   }
{
   surface_ = glfwManager_.createWindowSurface(instance_.handle(), window);
   if (surface_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create window surface.");
   }
}

Surface::~Surface()
{
   vulkanInterface_.destroySurface(instance_.handle(), surface_);
}

Surface::Surface(Surface&& other) :
   logger_{ other.logger_ }, glfwManager_{ other.glfwManager_ }, vulkanInterface_{ other.vulkanInterface_ }, instance_{ other.instance_ }
{
   surface_ = other.surface_;
   other.surface_ = VK_NULL_HANDLE;

   capabilities_ = other.capabilities_;
   formats_ = other.formats_;
   modes_ = other.modes_;
}

Surface& Surface::operator=(Surface&& other)
{
   if (surface_ != VK_NULL_HANDLE)
   {
      vulkanInterface_.destroySurface(instance_.handle(), surface_);
   }

   surface_ = other.surface_;
   other.surface_ = VK_NULL_HANDLE;

   capabilities_ = other.capabilities_;
   formats_ = other.formats_;
   modes_ = other.modes_;

   return *this;
}

VkSurfaceCapabilitiesKHR Surface::capabilities() const
{
   std::scoped_lock lck{ mtx_ };
   return capabilities_;
}

std::vector<VkSurfaceFormatKHR> Surface::formats() const
{
   std::scoped_lock lck{ mtx_ };
   return formats_;
}

VkSurfaceFormatKHR Surface::getBestFormat() const
{
   std::scoped_lock lck{ mtx_ };

   for (const auto& format : formats_)
   {
      if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
         return format;
      }
   }

   return formats_[0];
}

VkPresentModeKHR Surface::getBestMode() const
{
   std::scoped_lock lck{ mtx_ };

   for (const auto& mode : modes_)
   {
      if (mode == VK_PRESENT_MODE_MAILBOX_KHR) return mode;
   }

   return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceKHR Surface::handle()
{
   return surface_;
}

bool Surface::init(VkPhysicalDevice physicalDevice)
{
   capabilities_ = vulkanInterface_.getSurfaceCapabilities(physicalDevice, surface_);
   formats_ = vulkanInterface_.getSurfaceFormats(physicalDevice, surface_);
   modes_ = vulkanInterface_.getSurfacePresentationModes(physicalDevice, surface_);

   if (formats_.empty() || modes_.empty())
   {
      logger_.relErr("The current physical device does not support all features required by the used surface.");
      return false;
   }

   return true;
}

std::vector<VkPresentModeKHR> Surface::modes() const
{
   std::scoped_lock lck{ mtx_ };
   return modes_;
}