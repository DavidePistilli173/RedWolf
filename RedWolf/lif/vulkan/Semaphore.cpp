#include "Semaphore.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"

using namespace rw::lif::vlk;

Semaphore::Semaphore(RedWolfManager& manager, const DeviceBase& device) : BaseObject(manager), device_{ device }
{
   VkSemaphoreCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

   semaphore_ = device_.createSemaphore(createInfo);
   if (semaphore_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create semaphore.");
   }
}

Semaphore::Semaphore(Semaphore&& other) noexcept : BaseObject(other.manager_), semaphore_{ other.semaphore_ }, device_{ other.device_ }
{
   other.semaphore_ = VK_NULL_HANDLE;
}

Semaphore::~Semaphore()
{
   device_.destroySemaphore(semaphore_);
}

VkSemaphore Semaphore::handle() const
{
   return semaphore_;
}