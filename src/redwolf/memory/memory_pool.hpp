#pragma once

#include "redwolf/common.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/profiler.hpp"

#include <cstring>
#include <type_traits>
#include <unordered_map>

namespace rw {
    /**
     * @brief Memory categories.
     */
    enum class MemoryType : u8 {
        invalid,  /**< Invalid allocation type. */
        events,   /**< Event allocations. */
        renderer, /**< Renderer allocations. */
        engine,   /**< Generic engine allocation. */
        modules,  /**< User modules allocations. */
        app       /**< Generic application allocation. */
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
         * @param elem_num Number of elements that must fit in memory.
         * @return Raw pointer to the newly allocated memory.
         */
        template<typename T>
        [[nodiscard]] T* allocate(usize elem_num = 1U) {
            RW_PROFILE_SCOPE

            const usize alloc_size{ elem_num * sizeof(T) };
            size_ += alloc_size; // Lower bound, more bytes could be allocated.
            auto res{ allocations_.emplace(new (std::align_val_t(std::alignment_of_v<T>)) u8[alloc_size], alloc_size) };
            return reinterpret_cast<T*>(res.first->first);
        }

        /**
         * @brief Deallocate memory that was previously allocated by this memory pool.
         * @tparam Type the memory was allocated for.
         * @param memory Memory to deallocate.
         */
        template<typename T>
        void deallocate(T* memory) {
            RW_PROFILE_SCOPE

            if (nullptr == memory) {
                warn("Deallocating null pointer.");
                return;
            }

            const auto it{ allocations_.find(reinterpret_cast<u8*>(memory)) };
            if (it == allocations_.end()) {
                error("Deallocating memory from different pool.");
                return;
            }

            if (nullptr != memory) {
                delete[] it->first;
                size_ -= it->second;
                allocations_.erase(it);
            }
        }

        /**
         * @brief Reallocate some memory to expand or shrink it.
         * @tparam Type the memory was allocated for.
         * @param memory Pointer to the memory to reallocate.
         * @param new_elem_num New number of elements that must fit inside memory.
         * @return New pointer, valid for the new size.
         */
        template<typename T>
        [[nodiscard]] T* reallocate(T* memory, usize new_elem_num) {
            RW_PROFILE_SCOPE

            // Same as a new allocation.
            if (nullptr == memory) {
                return allocate<T>(new_elem_num);
            }

            const auto it{ allocations_.find(reinterpret_cast<u8*>(memory)) };
            if (it == allocations_.end()) {
                error("Reallocating memory from different pool.");
                return memory;
            }

            const usize curr_size{ it->second };
            const usize new_size{ new_elem_num * sizeof(T) };
            if (new_size == curr_size) {
                warn("Reallocating without changing size.");
                return memory;
            }

            auto new_it{ allocations_.emplace(new (std::align_val_t(std::alignment_of_v<T>)) u8[new_size], new_size) };
            if (new_size > curr_size) {
                size_ += (new_size - curr_size);
            } else {
                size_ -= (curr_size - new_size);
            }

            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memmove(new_it.first->first, memory, std::min(curr_size, new_size));
            } else {
                for (usize i{ 0U }; i < (std::min(curr_size, new_size) / sizeof(T)); ++i) {
                    new (reinterpret_cast<T*>(new_it.first->first) + i) T(std::move(memory[i]));
                }
            }

            delete[] it->first;
            allocations_.erase(it);
            return reinterpret_cast<T*>(new_it.first->first);
        }

        /**
         * @brief Get the memory type of the pool.
         */
        [[nodiscard]] MemoryType type() const {
            return type_;
        }

     private:
        MemoryType                     type_{ MemoryType::invalid }; /**< Category this memory pool belongs to. */
        usize                          size_{ 0U };                  /**< Currently allocated size estimate. [B] */
        std::unordered_map<u8*, usize> allocations_;                 /**< Current allocations. */
    };
} // namespace rw