#pragma once

#include "redwolf/common.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/profiler.hpp"

namespace rw {
    /**
     * @brief Memory categories.
     */
    enum class MemoryType : u8 {
        engine, /**< Generic engine allocation. */
        app     /**< Generic application allocation. */
    };

    /**
     * @brief Single pool of memory that can be used by the engine.
     * @todo Implement an actual memory pool / memory arena.
     */
    class MemoryPool {
     public:
        explicit MemoryPool(MemoryType type);

        /**
         * @brief Allocate enough memory to store a single object, but does not create the object itself.
         * @tparam T Type of object to store in the newly allocated memory.
         * @return Raw pointer to the newly allocated memory.
         */
        template<typename T>
        [[nodiscard]] T* allocate() {
            RW_PROFILE_SCOPE

            size_ += sizeof(T); // Lower bound, more bytes could be allocated.
            return reinterpret_cast<T*>(new (std::align_val_t(std::alignment_of_v<T>)) u8[sizeof(T)]);
        }

        /**
         * @brief Deallocate memory that was previously allocated by this memory pool.
         * @tparam Type the memory was allocated for.
         * @param memory Memory to deallocate.
         */
        template<typename T>
        void deallocate(T* memory) {
            RW_PROFILE_SCOPE

            if (nullptr != memory) {
                delete[] reinterpret_cast<u8*>(memory);
                size_ -= sizeof(T);
            } else {
                warn("Deallocating null pointer.");
            }
        }

     private:
        MemoryType type_{ MemoryType::engine }; /**< Category this memory pool belongs to. */
        usize      size_{ 0U };                 /**< Currently allocated size estimate. [B] */
    };
} // namespace rw