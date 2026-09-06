#pragma once

#include "redwolf/common.hpp"
#include "redwolf/logger.hpp"
#include "redwolf/profiler.hpp"

namespace rw {
    /**
     * @brief Linear, fixed-size allocator. It does not allow reuse of freed memory.
     */
    class LinearAllocator {
     public:
        /**
         * @brief total_size Total available size. [B]
         */
        explicit LinearAllocator(usize total_size);

        ~LinearAllocator();

        LinearAllocator(const LinearAllocator&)            = delete;
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        LinearAllocator(LinearAllocator&&)            = delete;
        LinearAllocator& operator=(LinearAllocator&&) = delete;

        /**
         * @brief Allocate enough memory to store a single object, but does not create the object itself.
         * @tparam T Type of object to store in the newly allocated memory.
         * @param elem_num Number of elements that must fit in memory.
         * @return Raw pointer to the newly allocated memory.
         */
        template<typename T>
        [[nodiscard]] T* allocate(usize elem_num = 1U) {
            RW_PROFILE_SCOPE

            const usize allocation_size{ elem_num * sizeof(T) };
            u8*         address{ &memory_[allocated_size_] };
            while (0 != (address % std::alignment_of_v<T>) ) {
                ++address;
            }

            const usize total_required_memory{ (address - &memory_[allocated_size_]) + allocation_size };
            if (total_size_ <= (address - memory_)) {
                error(
                    "Failed to allocate memory: not enough space. Available/Required: {}B/{}B",
                    total_size_ - allocated_size_,
                    total_required_memory);
                return nullptr;
            }

            allocated_size_ += total_required_memory;
            return reinterpret_cast<T*>(address);
        }

        /**
         * @brief Free all memory previously allocated from this allocator.
         */
        void deallocate_all();

     private:
        u8*   memory_{ nullptr };    /**< Actual memory.  */
        usize total_size_{ 0U };     /**< Total memory size. [B] */
        usize allocated_size_{ 0U }; /**< Currently allocated size. [B] */
    };
} // namespace rw