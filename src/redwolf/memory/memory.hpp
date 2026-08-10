#pragma once

#include "redwolf/common.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/memory/ptr.hpp"

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
            auto* instance{ instance_() };
            switch (type) {
            case MemoryType::engine:
                return Ptr<T>(&(instance->pool_engine_), std::forward<Args>(args)...);
                break;
            case MemoryType::app:
                return Ptr<T>(&(instance->pool_app_), std::forward<Args>(args)...);
                break;
            default:
                error("Invalid allocation type: '{}'", static_cast<u8>(type));
                return Ptr<T>();
                break;
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

        MemoryPool pool_engine_{ MemoryType::engine }; /**< Generic engine memory pool. */
        MemoryPool pool_app_{ MemoryType::app };       /**< Generic application memory pool. */
    };

} // namespace rw