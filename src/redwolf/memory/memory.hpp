#pragma once

#include "redwolf/common.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/memory/ptr.hpp"
#include "redwolf/profiler.hpp"

#include <utility>

namespace rw {

    /**
     * @brief Memory manager.
     */
    class Memory {
     public:
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
        [[nodiscard]] static Ptr<T> new_object(MemoryType type, Args&&... args) {
            RW_PROFILE_SCOPE

            auto* instance{ instance_() };
            switch (type) {
            case MemoryType::renderer:
                return Ptr<T>(&(instance->pool_renderer_), std::forward<Args>(args)...);
            case MemoryType::engine:
                return Ptr<T>(&(instance->pool_engine_), std::forward<Args>(args)...);
            case MemoryType::modules:
                return Ptr<T>(&(instance->pool_modules_), std::forward<Args>(args)...);
            case MemoryType::app:
                return Ptr<T>(&(instance->pool_app_), std::forward<Args>(args)...);
            case MemoryType::invalid:
            default:
                error("Invalid allocation type: '{}'", static_cast<u8>(type));
                return Ptr<T>(&(instance->pool_invalid_), std::forward<Args>(args)...);
            }
        }

        /**
         * @brief Get one of the engine's memory pools.
         * @param type Allocation category.
         * @return Reference to the requested memory pool.
         */
        [[nodiscard]] static MemoryPool& pool(MemoryType type);

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

        MemoryPool pool_invalid_{ MemoryType::invalid };   /**< Invalid memory pool, just to make the program not crash. */
        MemoryPool pool_renderer_{ MemoryType::renderer }; /**< Renderer memory pool. */
        MemoryPool pool_engine_{ MemoryType::engine };     /**< Generic engine memory pool. */
        MemoryPool pool_modules_{ MemoryType::modules };   /**< User modules memory pool. */
        MemoryPool pool_app_{ MemoryType::app };           /**< Generic application memory pool. */
    };

} // namespace rw