#pragma once

#include "generic_allocator.hpp"
#include "ptr.hpp"
#include "redwolf/common.hpp"
#include "redwolf/profiler.hpp"

#include <utility>

namespace rw {
    /**
     * @brief Memory categories.
     */
    enum class MemoryCategory : u8 {
        invalid,  /**< Invalid allocation type. */
        events,   /**< Event allocations. */
        renderer, /**< Renderer allocations. */
        engine,   /**< Generic engine allocation. */
        modules,  /**< User modules allocations. */
        app       /**< Generic application allocation. */
    };

    /**
     * @brief Memory manager.
     */
    class Memory {
     public:
        /**
         * @brief Get one of the engine's allocators.
         * @param type Allocation category.
         * @return Reference to the requested memory pool.
         */
        [[nodiscard]] static GenericAllocator& allocator(MemoryCategory type);

        /**
         * @brief Initialise the memory manager.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Create a new dynamically allocated object.
         * @tparam T Type of object to create.
         * @tparam Args Construction arguments for the object.
         * @param type Allocation category.
         * @param args Construction arguments.
         */
        template<typename T, typename... Args>
        [[nodiscard]] static Ptr<T> new_object(MemoryCategory type, Args&&... args) {
            RW_PROFILE_SCOPE

            auto* instance{ instance_() };
            switch (type) {
            case rw::MemoryCategory::events:
                return Ptr<T>(&(instance->pool_events_), std::forward<Args>(args)...);
            case MemoryCategory::renderer:
                return Ptr<T>(&(instance->pool_renderer_), std::forward<Args>(args)...);
            case MemoryCategory::engine:
                return Ptr<T>(&(instance->pool_engine_), std::forward<Args>(args)...);
            case MemoryCategory::modules:
                return Ptr<T>(&(instance->pool_modules_), std::forward<Args>(args)...);
            case MemoryCategory::app:
                return Ptr<T>(&(instance->pool_app_), std::forward<Args>(args)...);
            case MemoryCategory::invalid:
            default:
                error("Invalid allocation type: '{}'", static_cast<u8>(type));
                return Ptr<T>(&(instance->pool_invalid_), std::forward<Args>(args)...);
            }
        }

        /**
         * @brief De-initialise the memory system.
         */
        static void shutdown();

     private:
        Memory() = default;

        /**
         * @brief Get the memory manager's instance.
         */
        [[nodiscard]] static Memory* instance_();

        GenericAllocator pool_invalid_;  /**< Invalid memory pool, just to make the program not crash. */
        GenericAllocator pool_events_;   /**< Events memory pool. */
        GenericAllocator pool_renderer_; /**< Renderer memory pool. */
        GenericAllocator pool_engine_;   /**< Generic engine memory pool. */
        GenericAllocator pool_modules_;  /**< User modules memory pool. */
        GenericAllocator pool_app_;      /**< Generic application memory pool. */
    };

} // namespace rw