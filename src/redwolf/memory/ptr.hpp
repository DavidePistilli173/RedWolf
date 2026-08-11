#pragma once

#include "redwolf/memory/memory.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/profiler.hpp"

#include <atomic>

namespace rw {
    /**
     * @brief Custom smart pointer similar to std::shared_ptr. Thread-safe.
     */
    template<typename T>
    class Ptr {
     public:
        friend class Memory;

        /**
         * @brief Memory block that will be allocated.
         */
        struct Block {
            std::atomic<u32> ref_count{ 1U };
            T                data_{};
        };

        /**
         * @brief Destructor.
         */
        ~Ptr() {
            reset();
        }

        /**
         * @brief Copy constructor.
         */
        Ptr(const Ptr& other) : pool_{ other.pool_ }, block_{ other.block_ } {
            RW_PROFILE_SCOPE

            if (nullptr != block_) {
                ++(block_->ref_count_);
            }
        }

        /**
         * @brief Copy-assignment operator.
         */
        Ptr& operator=(const Ptr& other) {
            RW_PROFILE_SCOPE

            if (this != &other) {
                reset();
                pool_  = other.pool_;
                block_ = other.block_;

                if (nullptr != block_) {
                    ++(block_->ref_count_);
                }
            }
            return *this;
        }

        /**
         * @brief Move constructor.
         */
        Ptr(Ptr&& other) noexcept : pool_{ other.pool_ }, block_{ other.block_ } {
            RW_PROFILE_SCOPE

            if (nullptr != block_) {
                ++(block_->ref_count_);
            }
            other.reset();
        }

        /**
         * @brief Move-assignment operator.
         */
        Ptr& operator=(Ptr&& other) noexcept {
            RW_PROFILE_SCOPE

            if (this != other) {
                reset();
                pool_  = other.pool_;
                block_ = other.block_;

                if (nullptr != block_) {
                    ++(block_->ref_count);
                }
                other.reset();
            }
            return *this;
        }

        /**
         * @brief Dereference operator*.
         */
        T& operator*() {
            return block_->data_;
        }

        /**
         * @brief Structure dereference operator->.
         */
        T* operator->() {
            return &(block_->data_);
        }

        /**
         * @brief Get the raw pointer.
         */
        [[nodiscard]] T* get() {
            return &(block_->data_);
        }

        /**
         * @brief Reset the pointer.
         */
        void reset() {
            RW_PROFILE_SCOPE

            if (nullptr == block_) {
                return;
            }

            const u32 prev_ref_count{ block_->ref_count.fetch_sub(1) };
            if (0 == prev_ref_count) {
                error("Something went wrong with the reference counter.");
            }
            if (1 == prev_ref_count) {
                block_->data_.~T();
                pool_->deallocate<Block>(block_);
            }
            block_ = nullptr;
        }

     protected:
        explicit Ptr() = default;

        /**
         * @brief Constructor.
         */
        template<typename... Args>
        explicit Ptr(MemoryPool* pool, Args&&... args) : pool_{ pool }, block_{ pool_->allocate<Block>() } {
            RW_PROFILE_SCOPE

            new (&block_->ref_count) std::atomic<u32>(1U);
            new (&block_->data_) T(std::forward<Args>(args)...);
        }

     private:
        MemoryPool* pool_{ nullptr };  /**< Memory pool that owns the allocated memory. */
        Block*      block_{ nullptr }; /**< Allocated memory block. */
    };
} // namespace rw