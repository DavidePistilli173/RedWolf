#pragma once

#include "redwolf/memory/memory.hpp"
#include "redwolf/memory/memory_pool.hpp"
#include "redwolf/profiler.hpp"

#include <atomic>

namespace rw {
    /**
     * @brief Base class for the control block of Ptr.
     */
    class ControlBlockBase {
     public:
        ControlBlockBase()                                   = default;
        virtual ~ControlBlockBase()                          = default;
        ControlBlockBase(const ControlBlockBase&)            = delete;
        ControlBlockBase& operator=(const ControlBlockBase&) = delete;
        ControlBlockBase(ControlBlockBase&&)                 = delete;
        ControlBlockBase& operator=(ControlBlockBase&&)      = delete;

        /**
         * @brief Destroy and deallocate the control block.
         */
        virtual void destroy() = 0;

        std::atomic<u32> ref_count{ 1U };
    };

    /**
     * @brief Actual control block class for Ptr.
     */
    template<typename T>
    class ControlBlock final : public ControlBlockBase {
     public:
        template<typename... Args>
        explicit ControlBlock(MemoryPool* pool, Args&&... args) : pool_{ pool }, data_{ std::forward<Args>(args)... } {}

        /**
         * @brief Destroy and deallocate the control block.
         */
        void destroy() override {
            data_.~T();
            pool_->deallocate<ControlBlock<T>>(this);
        }

        [[nodiscard]] T* get() {
            return &data_;
        }

     private:
        MemoryPool* pool_;
        T           data_;
    };

    /**
     * @brief Custom smart pointer similar to std::shared_ptr. Thread-safe.
     */
    template<typename T>
    class Ptr {
     public:
        template<typename U>
        friend class Ptr;

        friend class Memory;

        template<typename To, typename From>
        friend Ptr<To> static_ptr_cast(const Ptr<From>&);

        template<typename To, typename From>
        friend Ptr<To> dynamic_ptr_cast(const Ptr<From>&);

        explicit Ptr() = default;

        /**
         * @brief Destructor.
         */
        ~Ptr() {
            reset();
        }

        /**
         * @brief Copy constructor.
         */
        Ptr(const Ptr& other) : control_{ other.control_ }, ptr_{ other.ptr_ } {
            RW_PROFILE_SCOPE

            if (nullptr != control_) {
                ++(control_->ref_count);
            }
        }

        /**
         * @brief Converting copy constructor. Enabled only for implicit upcasts.
         */
        template<typename U>
            requires std::is_convertible_v<U*, T*>
        Ptr(const Ptr<U>& other) : control_{ other.control_ }, ptr_{ other.ptr_ } {
            if (nullptr != control_) {
                ++(control_->ref_count);
            }
        }

        /**
         * @brief Copy-assignment operator.
         */
        Ptr& operator=(const Ptr& other) {
            RW_PROFILE_SCOPE

            if (this != &other) {
                reset();
                control_ = other.control_;
                ptr_     = other.ptr_;

                ++(control_->ref_count);
            }
            return *this;
        }

        /**
         * @brief Converting copy-assignment operator, enabled only for implicit upcasts.
         */
        template<typename U>
            requires std::is_convertible_v<U*, T*>
        Ptr& operator=(const Ptr& other) {
            RW_PROFILE_SCOPE

            if (this != &other) {
                reset();
                control_ = other.control_;
                ptr_     = other.ptr_;

                ++(control_->ref_count);
            }
            return *this;
        }

        /**
         * @brief Move constructor.
         */
        Ptr(Ptr&& other) noexcept : control_{ other.control_ }, ptr_{ other.ptr_ } {
            RW_PROFILE_SCOPE

            other.control_ = nullptr;
            other.ptr_     = nullptr;
        }

        /**
         * @brief Converting move constructor. Enabled only for implicit upcasts.
         */
        template<typename U>
            requires std::is_convertible_v<U*, T*>
        Ptr(Ptr<U>&& other) noexcept : control_{ other.control_ }, ptr_{ other.ptr_ } {
            other.control_ = nullptr;
            other.ptr_     = nullptr;
        }

        /**
         * @brief Move-assignment operator.
         */
        Ptr& operator=(Ptr&& other) noexcept {
            RW_PROFILE_SCOPE

            if (this != &other) {
                reset();
                control_       = other.control_;
                other.control_ = nullptr;

                ptr_       = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        /**
         * @brief Converting move-assignment operator. Only enabled for implicit upcasts.
         */
        template<typename U>
            requires std::is_convertible_v<U*, T*>
        Ptr& operator=(Ptr&& other) noexcept {
            RW_PROFILE_SCOPE

            if (this != &other) {
                reset();
                control_       = other.control_;
                other.control_ = nullptr;

                ptr_       = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        /**
         * @brief Dereference operator*.
         */
        T& operator*() const {
            return *ptr_;
        }

        /**
         * @brief Structure dereference operator->.
         */
        T* operator->() const {
            return ptr_;
        }

        /**
         * @brief Get the raw pointer.
         */
        [[nodiscard]] T* get() const {
            return ptr_;
        }

        /**
         * @brief Check if the pointer is null or not.
         * @return true if the pointer is null, false otherwise.
         */
        [[nodiscard]] bool is_null() {
            return nullptr == ptr_;
        }

        /**
         * @brief Reset the pointer.
         */
        void reset() {
            RW_PROFILE_SCOPE

            if (nullptr == control_) {
                return;
            }

            const u32 prev_ref_count{ control_->ref_count.fetch_sub(1) };
            if (0 == prev_ref_count) {
                error("Something went wrong with the reference counter.");
            }
            if (1 == prev_ref_count) {
                control_->destroy();
            }
            control_ = nullptr;
            ptr_     = nullptr;
        }

     protected:
        /**
         * @brief Constructor.
         */
        template<typename... Args>
        explicit Ptr(MemoryPool* pool, Args&&... args) {
            RW_PROFILE_SCOPE

            auto* block{ pool->allocate<ControlBlock<T>>() };
            new (block) ControlBlock<T>(pool, std::forward<Args>(args)...);
            control_ = block;
            ptr_     = block->get();
        }

     private:
        ControlBlockBase* control_{ nullptr }; /**< Control block. */
        T*                ptr_{ nullptr };     /**< Convenience access to the managed object. */
    };

    /**
     * @brief Static cast of a pointer to another.
     */
    template<typename To, typename From>
    [[nodiscard]] Ptr<To> static_ptr_cast(const Ptr<From>& other) {
        RW_PROFILE_SCOPE

        Ptr<To> result;
        result.control_ = other.control_;
        result.ptr_     = static_cast<To*>(other.ptr_);
        if (nullptr != result.control_) {
            ++(result.control->ref_count);
        }
        return result;
    }

    template<typename To, typename From>
    [[nodiscard]] Ptr<To> dynamic_ptr_cast(const Ptr<From>& other) {
        RW_PROFILE_SCOPE

        To* casted{ dynamic_cast<To*>(other.ptr_) };
        if (nullptr == casted) {
            return {};
        }

        Ptr<To> result;
        result.control_ = other.control_;
        result.ptr_     = casted;
        ++(result.control_->ref_count);
        return result;
    }
} // namespace rw