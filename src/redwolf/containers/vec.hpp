#pragma once

#include "redwolf/common.hpp"
#include "redwolf/memory/memory.hpp"

#include <algorithm>
#include <concepts>

namespace rw {
    /**
     * @brief Custom dynamically sized vector, built to use the engine's memory management system.
     */
    template<typename T>
    class Vec {
     public:
        static constexpr usize growth_factor{ 2 }; /**< Memory growth when automatic reallocation is needed. */

        /**
         * @brief Constructor.
         * @param memory_type Type of memory where the data will be stored.
         */
        explicit Vec(MemoryType memory_type) : memory_pool_{ &Memory::pool(memory_type) } {}

        /**
         * @brief Destructor.
         */
        ~Vec() {
            reset_();
        }

        // Deleted because shallow copies are not allowed.
        // Deep copies are allowed through the clone method to be more explicit.
        Vec(const Vec&)            = delete;
        Vec& operator=(const Vec&) = delete;

        /**
         * @brief Move constructor.
         */
        Vec(Vec&& other) noexcept :
            memory_pool_{ other.memory_pool_ }, elements_{ other.elements_ }, size_{ other.size_ }, capacity_{ other.capacity_ } {
            other.elements_ = nullptr;
            other.size_     = 0;
            other.capacity_ = 0;
        }

        /**
         * @brief Move-assignment operator.
         */
        Vec& operator=(Vec&& other) noexcept {
            if (this != &other) {
                reset_();

                memory_pool_ = other.memory_pool_;

                elements_       = other.elements_;
                other.elements_ = nullptr;

                size_       = other.size_;
                other.size_ = 0;

                capacity_       = other.capacity_;
                other.capacity_ = 0;
            }
            return *this;
        }

        /**
         * @brief Subscript operator.
         * @details No check is performed on the index.
         */
        const T& operator[](usize index) const {
            return elements_[index];
        }

        /**
         * @brief Subscript operator.
         * @details No check is performed on the index.
         */
        T& operator[](usize index) {
            return elements_[index];
        }

        /**
         * @brief Get a reference to the last element of the vector.
         * @details No check is performed on the validity of such element.
         */
        [[nodiscard]] const T& back() const {
            return elements_[size_ - 1];
        }

        /**
         * @brief Get a reference to the last element of the vector.
         * @details No check is performed on the validity of such element.
         */
        [[nodiscard]] T& back() {
            return elements_[size_ - 1];
        }

        /**
         * @brief Get the current vector capacity (number of elements).
         */
        [[nodiscard]] usize capacity() const {
            return capacity_;
        }

        /**
         * @brief Clears all elements, without deallocating any memory.
         */
        void clear() {
            for (usize i{ 0U }; i < size_; ++i) {
                elements_[i].~T();
            }
        }

        /**
         * @brief Clone the vector and all its elements.
         * @return Cloned vector.
         */
        [[nodiscard]] Vec clone() const {
            Vec new_vec;
            new_vec.memory_pool_ = memory_pool_;
            new_vec.reserve(size_);
            for (usize i{ 0 }; i < size_; ++i) {
                new (&(new_vec->elements_[i])) T(elements_[i]);
            }
            new_vec.size_ = size_;
            return new_vec;
        }

        /**
         * @brief Get the raw pointer to the contiguous memory of the vector.
         */
        [[nodiscard]] const T* data() const {
            return elements_;
        }

        /**
         * @brief Get the raw pointer to the contiguous memory of the vector.
         */
        [[nodiscard]] T* data() {
            return elements_;
        }

        template<typename... Args>
        [[nodiscard]] T& emplace_back(Args&&... args) {
            // Needs reallocation.
            if (capacity_ == size_) {
                const usize new_size{ std::max(static_cast<usize>(1), size_ * growth_factor) };
                elements_ = memory_pool_->reallocate(elements_, new_size);
                trace("elements_: '{:x}'", (usize) elements_);
                capacity_ = new_size;
            }

            auto* new_element{ new (&(elements_[size_])) T(std::forward<Args>(args)...) };
            ++size_;
            return *new_element;
        }

        /**
         * @brief Check whether the vector is empty or not.
         * @return true if the vector is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const {
            return 0 == size_;
        }

        /**
         * @brief Erase an element at a given index.
         * @param index Index of the element to erase.
         */
        void erase(usize index) {
            if (index >= size_) {
                warn("Erasing non-existent element.");
                return;
            }

            shift_elements_(index, index + 1);
        }

        /**
         * @brief Erase all elements in the vector that satisfy a given predicate.
         * @tparam FnT Predicate in the form bool (const T& value). If true is returned for an element, that element is erased.
         */
        template<typename FnT>
            requires requires(FnT fn, const T& t) {
                { fn(t) } -> std::same_as<bool>;
            }
        void erase_all(FnT fn) {
            usize index{ 0U };
            while (index < size_) {
                if (fn(elements_[index])) {
                    erase(index);
                } else {
                    ++index;
                }
            }
        }

        /**
         * @brief Erase all elements in the vector that satisfy a given predicate.
         * @tparam FnT Predicate in the form bool (const T& value). If true is returned for an element, that element is erased.
         */
        template<typename FnT>
            requires requires(FnT fn, const T& t) {
                { fn(t) } -> std::same_as<bool>;
            }
        void erase_first(FnT fn) {
            usize index{ 0U };
            while (index < size_) {
                if (fn(elements_[index])) {
                    erase(index);
                    return;
                }
                ++index;
            }
        }

        /**
         * @brief Get a reference to the first element of the vector.
         * @details No check is performed on the validity of such element.
         */
        [[nodiscard]] const T& front() const {
            return elements_[0];
        }

        /**
         * @brief Get a reference to the first element of the vector.
         * @details No check is performed on the validity of such element.
         */
        [[nodiscard]] T& front() {
            return elements_[0];
        }

        /**
         * @brief Reserve a specific size in the vector capacity.
         * @param size Number of elements that need to fit in the new capacity.
         */
        void reserve(usize size) {
            // Vector already large enough: nothing to do.
            if (capacity_ >= size) {
                return;
            }

            elements_ = memory_pool_->reallocate(elements_, size);
            capacity_ = size;
        }

        /**
         * @brief Resize the vector to a given size..
         * @param new_size New number of elements of the vector. If larger than the current size, new elements will be created. If smaller,
         * the excess elements will be deleted.
         * @param value Value to initialise the new elements with.
         */
        void resize(usize new_size, T value = T{}) {
            // Same size, nothing to do.
            if (new_size == size_) {
                return;
            }

            // Shrink the vector.
            if (new_size < size_) {
                for (usize i{ size_ - 1 }; i >= new_size; --i) {
                    elements_[i].~T();
                }
                size_ = new_size;
                return;
            }

            // Expand the vector.
            elements_ = memory_pool_->reallocate(elements_, new_size);
            for (; size_ < new_size; ++size_) {
                new (&(elements_[size_])) T(value);
            }
        }

        /**
         * @brief Shrink the vector to fit the current number of elements exactly.
         */
        void shrink_to_fit() {
            // Vector already small enough: nothing to do.
            if (capacity_ == size_) {
                return;
            }

            elements_ = memory_pool_->reallocate(elements_, size_);
            capacity_ = size_;
        }

        /**
         * @brief Get the current size of the vector (number of elements).
         */
        [[nodiscard]] usize size() const {
            return size_;
        }

     private:
        Vec() = default;

        /**
         * @brief Reset the vector, deallocating all its memory.
         */
        void reset_() {
            if (nullptr != elements_) {
                clear();
                memory_pool_->deallocate(elements_);
                elements_ = nullptr;
            }
        }

        /**
         * @brief Shift all the elements in a given range up or down the vector.
         * @param dst_index Index where the first element of the shifted range will go.
         * @param src_index Index of the first element to move.
         */
        void shift_elements_(usize dst_index, usize src_index) {
            if (dst_index < src_index) {
                usize curr_dst_index{ dst_index };
                usize curr_src_index{ src_index };
                elements_[curr_dst_index].~T();

                while (curr_src_index < size_) {
                    new (&(elements_[curr_dst_index])) T(std::move(elements_[curr_src_index]));
                    ++curr_dst_index;
                    ++curr_src_index;
                }
                size_ -= (src_index - dst_index);

            } else {
                usize size_diff{ dst_index - src_index };
                usize new_potential_size{ size_ + size_diff };
                if (new_potential_size > capacity_) {
                    elements_ = memory_pool_->reallocate(elements_, new_potential_size);
                    size_     = new_potential_size;
                }

                usize curr_dst_index{ size_ - 1 };
                usize curr_src_index{ curr_dst_index - size_diff };
                while (curr_src_index >= src_index) {
                    new (&(elements_[curr_dst_index])) T(std::move(elements_[curr_src_index]));
                    --curr_dst_index;
                    --curr_src_index;
                }
            }
        }

        MemoryPool* memory_pool_{ nullptr }; /**< Memory pool where the data is allocated. */
        T*          elements_{ nullptr };    /**< Actual vector elements. */
        usize       size_{ 0U };             /**< Number of elements in the vector. */
        usize       capacity_{ 0U };         /**< Current allocation capacity. */
    };
} // namespace rw