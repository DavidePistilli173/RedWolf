#pragma once

#include "redwolf/common.hpp"

#include <array>
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

namespace rw {
    /**
     * @brief Allocation categories.
     */
    enum class AllocType : u8 {
        unknown,    /**< Unknown category. */
        engine,     /**< Generic engine allocation. */
        application /**< Generic application allocation. */
    };

    /**
     * @brief Memory allocation information.
     */
    struct AllocInfo {
        std::atomic<u32> ref_count{ 0 };             /**< Reference count for this allocation. */
        AllocType        type{ AllocType::unknown }; /**< Allocation category. */
    };

    /**
     * @brief Custom smart pointer similar to std::shared_ptr. Thread-safe.
     */
    template<typename T>
    class Ptr {
     public:
        friend class Memory;

        /**
         * @brief Destructor.
         */
        ~Ptr() {
            reset();
        }

        /**
         * @brief Copy constructor.
         */
        Ptr(const Ptr& other) : ptr_{ other.ptr_ }, ref_count_{ other.ref_count_ } {
            ++(*ref_count_);
        }

        /**
         * @brief Copy-assignment operator.
         */
        Ptr& operator=(const Ptr& other) {
            if (this != &other) {
                reset();
                ptr_       = other.ptr_;
                ref_count_ = other.ref_count_;
                ++(*ref_count_);
            }
            return *this;
        }

        /**
         * @brief Move constructor.
         */
        Ptr(Ptr&& other) noexcept : ptr_{ other.ptr_ }, ref_count_{ other.ref_count_ } {
            ++(*ref_count_);
            other.reset();
        }

        /**
         * @brief Move-assignment operator.
         */
        Ptr& operator=(Ptr&& other) noexcept {
            if (this != other) {
                reset();
                ptr_       = other.ptr_;
                ref_count_ = other.ref_count_;
                ++(*ref_count_);
                other.reset();
            }
            return *this;
        }

        /**
         * @brief Dereference operator*.
         */
        T& operator*() {
            return *ptr_;
        }

        /**
         * @brief Structure dereference operator->.
         */
        T* operator->() {
            return ptr_;
        }

        /**
         * @brief Get the raw pointer.
         */
        [[nodiscard]] T* get() {
            return ptr_;
        }

        /**
         * @brief Reset the pointer.
         */
        void reset() {
            const u32 prev_ref_count{ ref_count_->fetch_sub(1) };
            if (1 == prev_ref_count) {
                delete ptr_;
            }
            ptr_ = nullptr;
        }

     protected:
        /**
         * @brief Constructor.
         */
        template<typename... Args>
        explicit Ptr(std::atomic<u32>* ref_count, Args&&... args) : ptr_{ new T(std::forward<Args>(args)...) }, ref_count_{ ref_count } {}

     private:
        T*                ptr_{ nullptr }; /**< Pointer to the actual data. */
        std::atomic<u32>* ref_count_;      /**< Reference count. */
    };

    /**
     * @brief Memory manager.
     */
    class Memory {
     public:
        static constexpr usize page_alloc_num{ 1'000 }; /**< Number of allocations per page. */

        /**
         * @brief Initialise the memory manager.
         */
        [[nodiscard]] static bool init();

        template<typename T, typename... Args>
        [[nodiscard]] Ptr<T> new_object(Args&&... args) {
            return Ptr<T>(std::forward<Args>(args)...);
        }

     private:
        Memory() = default;
    };

} // namespace rw