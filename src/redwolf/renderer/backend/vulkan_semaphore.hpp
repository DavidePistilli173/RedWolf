#pragma once

#include "vulkan_device.hpp"

#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan semaphore wrapper.
     */
    class Semaphore {
     public:
        Semaphore() = default;
        ~Semaphore();

        Semaphore(const Semaphore&)            = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        Semaphore(Semaphore&&)            = delete;
        Semaphore& operator=(Semaphore&&) = delete;

        /**
         * @brief Get the raw semaphore handle.
         */
        [[nodiscard]] VkSemaphore handle() const;

        /**
         * @brief Get a pointer to the raw semaphore handle.
         */
        [[nodiscard]] VkSemaphore* handle_pointer();

        /**
         * @brief Initialise the semaphore.
         * @param allocator Custom vulkan allocator.
         * @param device Rendering device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(VkAllocationCallbacks* allocator, Ptr<Device> device);

     private:
        VkAllocationCallbacks* allocator_{ nullptr };        /**< Custom vulkan allocator. */
        Ptr<Device>            device_;                      /**< Rendering device. */
        VkSemaphore            semaphore_{ VK_NULL_HANDLE }; /**> Raw semaphore handle. */
    };
} // namespace rw::vk