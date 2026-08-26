#pragma once

#include "vulkan_device.hpp"

#include <chrono>
#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan fence wrapper.
     */
    class Fence {
     public:
        Fence() = default;
        ~Fence();

        Fence(const Fence&)            = delete;
        Fence& operator=(const Fence&) = delete;

        Fence(Fence&&)            = delete;
        Fence& operator=(Fence&&) = delete;

        /**
         * @brief Get the raw fence handle.
         */
        [[nodiscard]] VkFence handle() const;

        /**
         * @brief Initialise the fence.
         * @param allocator Custom vulkan allocator.
         * @param device Rendering device.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(VkAllocationCallbacks* allocator, Ptr<Device> device, bool starts_signaled);

        /**
         * @brief Reset the fence to its unsignalled state.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool reset();

        /**
         * @brief Wait for the fence to be signalled.
         * @param timeout Timeout for the wait.
         * @return true if the wait was successfull and the fence was signalled within the timeout, false otherwise.
         */
        [[nodiscard]] bool wait(std::chrono::nanoseconds timeout);

     private:
        VkAllocationCallbacks* allocator_{ nullptr };    /**< Custom vulkan allocator. */
        Ptr<Device>            device_;                  /**< Rendering device. */
        VkFence                fence_{ VK_NULL_HANDLE }; /**< Raw fence handle. */
        bool                   signaled_{ false };       /**< If true, the fence has been signalled. */
    };
} // namespace rw::vk