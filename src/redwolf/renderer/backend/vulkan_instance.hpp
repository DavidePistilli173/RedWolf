#pragma once

#include "redwolf/containers/vec.hpp"

#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan instance wrapper.
     */
    class Instance {
     public:
        Instance() = default;
        ~Instance();

        Instance(const Instance&)            = delete;
        Instance& operator=(const Instance&) = delete;

        Instance(Instance&&)            = delete;
        Instance& operator=(Instance&&) = delete;

        /**
         * @brief Get the raw instance handle.
         */
        [[nodiscard]] VkInstance handle() const;

        /**
         * @brief Initialise the instance.
         * @param allocator Custom vulkan allocator.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(VkAllocationCallbacks* allocator);

     private:
        /**
         * @brief Initialise the Vulkan debugger.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_debugger_();

        /**
         * @brief Initialise the Vulkan instance.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init_instance_();

        /**
         * @brief Initialise the Vulkan layer names.
         * @return Vector containing all required layer names.
         */
        [[nodiscard]] static Vec<const char*> init_layer_names_();

        VkInstance             instance_{ VK_NULL_HANDLE }; /**< Raw instance handle. */
        VkAllocationCallbacks* allocator_{ nullptr };       /**< Custom Vulkan allocator. */

#ifdef RW_ENABLE_VULKAN_DEBUG
        VkDebugUtilsMessengerEXT debug_messenger_{}; /**< Vulkan debug messenger. */
#endif
    };
} // namespace rw::vk