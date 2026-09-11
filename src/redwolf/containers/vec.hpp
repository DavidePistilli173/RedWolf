#pragma once

#include "iterators.hpp"
#include "redwolf/common.hpp"
#include "redwolf/memory/memory.hpp"
#include "view.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <format>
#include <string_view>

namespace rw {
    /**
     * @brief Custom dynamically sized vector, built to use the engine's memory management system.
     */
    template<typename T>
    class Vec {
     public:
        static constexpr usize growth_factor{ 2 }; /**< Memory growth when automatic reallocation is needed. */
        static constexpr usize min_capacity{ 8 };  /**< Minimum number of items that can be stored. */

        /**
         * @brief Constructor.
         * @param memory_category Type of memory where the data will be stored.
         */
        explicit Vec(MemoryCategory memory_category) : allocator_{ &Memory::allocator(memory_category) } {}

        /**
         * @brief Construct a vector with some initial data.
         * @param memory_category Type of memory where the data will be stored.
         * @param initial_data Data to initialise the vector with.
         */
        template<size_t Size>
        Vec(MemoryCategory memory_category, const std::array<T, Size> initial_data) : allocator_{ &Memory::allocator(memory_category) } {
            reserve(Size);
            for (const auto& elem : initial_data) {
                (void) emplace_back(elem);
            }
        }

        /**
         * @brief Destructor.
         */
        ~Vec() {
            reset();
        }

        // Deleted because shallow copies are not allowed.
        // Deep copies are allowed through the clone method to be more explicit.
        Vec(const Vec&)            = delete;
        Vec& operator=(const Vec&) = delete;

        /**
         * @brief Move constructor.
         */
        Vec(Vec&& other) noexcept :
            allocator_{ other.allocator_ }, elements_{ other.elements_ }, size_{ other.size_ }, capacity_{ other.capacity_ } {
            other.elements_ = nullptr;
            other.size_     = 0;
            other.capacity_ = 0;
        }

        /**
         * @brief Move-assignment operator.
         */
        Vec& operator=(Vec&& other) noexcept {
            if (this != &other) {
                reset();

                allocator_ = other.allocator_;

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
         * @brief Append some data to the vector.
         */
        void append(View<T> data) {
            reserve(size_ + data.size());

            for (const auto& element : data) {
                (void) emplace_back(element);
            }
        }

        /**
         * @brief Append some data to the vector.
         */
        void append(const Vec<T>& data) {
            reserve(size_ + data.size());

            for (const auto& element : data) {
                (void) emplace_back(element);
            }
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
         * @brief Get an iterator to the first element of the vector.
         */
        [[nodiscard]] ContiguousIterator<T, false> begin() {
            return ContiguousIterator<T, false>(elements_);
        }

        /**
         * @brief Get an iterator to the first element of the vector.
         */
        [[nodiscard]] ContiguousIterator<T, true> begin() const {
            return ContiguousIterator<T, true>(elements_);
        }

        /**
         * @brief Get the current vector capacity (number of elements).
         */
        [[nodiscard]] usize capacity() const {
            return capacity_;
        }

        /**
         * @brief Get an iterator to the first element of the vector.
         */
        [[nodiscard]] ContiguousIterator<T, true> cbegin() const {
            return ContiguousIterator(elements_);
        }

        /**
         * @brief Get the end iterator of the vector. (One item past the last valid item.)
         */
        [[nodiscard]] ContiguousIterator<T, true> cend() const {
            return ContiguousIterator(elements_ + size_);
        }

        /**
         * @brief Clears all elements, without deallocating any memory.
         */
        void clear() {
            for (usize i{ 0U }; i < size_; ++i) {
                elements_[i].~T();
            }
            size_ = 0;
        }

        /**
         * @brief Clone the vector and all its elements.
         * @return Cloned vector.
         */
        [[nodiscard]] Vec clone() const {
            Vec new_vec;
            new_vec.allocator_ = allocator_;
            new_vec.reserve(size_);
            for (usize i{ 0 }; i < size_; ++i) {
                new (&(new_vec.elements_[i])) T(elements_[i]);
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
                const usize new_size{ std::max(min_capacity, size_ * growth_factor) };
                elements_ = allocator_->reallocate(elements_, new_size);
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
         * @brief Get the end iterator of the vector. (One item past the last valid item.)
         */
        [[nodiscard]] ContiguousIterator<T, false> end() {
            return ContiguousIterator<T, false>(elements_ + size_);
        }

        /**
         * @brief Get the end iterator of the vector. (One item past the last valid item.)
         */
        [[nodiscard]] ContiguousIterator<T, true> end() const {
            return ContiguousIterator<T, true>(elements_ + size_);
        }

        /**
         * @brief Fill the vector with the given value.
         */
        void fill(const T& value) {
            for (usize i{ 0U }; i < size_; ++i) {
                elements_[i].~T();
                elements_[i] = value;
            }
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
         * @brief Erase the first element in the vector that satisfies a given predicate.
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
         * @brief Find the first in the vector that satisfy a given predicate.
         * @tparam FnT Predicate in the form bool (const T& value). If true is returned for an element, that element is found.
         * @return Index of the found element.
         */
        template<typename FnT>
            requires requires(FnT fn, const T& t) {
                { fn(t) } -> std::same_as<bool>;
            }
        [[nodiscard]] std::optional<usize> find_first(FnT fn) {
            usize index{ 0U };
            while (index < size_) {
                if (fn(elements_[index])) {
                    return index;
                }
                ++index;
            }
            return {};
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
         * @brief Reset the vector, deallocating all its memory.
         */
        void reset() {
            if (nullptr != elements_) {
                clear();
                allocator_->deallocate(elements_);
                elements_ = nullptr;
            }
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

            elements_ = allocator_->reallocate(elements_, size);
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
            if (new_size > capacity_) {
                elements_ = allocator_->reallocate(elements_, new_size);
                capacity_ = new_size;
            }
            for (; size_ < new_size; ++size_) {
                new (&(elements_[size_])) T(value);
            }

            size_ = new_size;
        }

        /**
         * @brief Shrink the vector to fit the current number of elements exactly.
         */
        void shrink_to_fit() {
            // Vector already small enough: nothing to do.
            if (capacity_ == size_) {
                return;
            }

            elements_ = allocator_->reallocate(elements_, size_);
            capacity_ = size_;
        }

        /**
         * @brief Get the current size of the vector (number of elements).
         */
        [[nodiscard]] usize size() const {
            return size_;
        }

        /**
         * @brief Get a sub-view over the vector data.
         * @details The parameters are note checked.
         * @param index Starting index of the subview.
         * @param size Number of elements of the subview.
         */
        [[nodiscard]] View<T> subview(usize index, usize size) {
            return View<T>(elements_ + index, size);
        }

        /**
         * @brief Cast the vector to have a different underlying type.
         */
        template<typename U>
        [[nodiscard]] Vec<U>& to() {
            return *reinterpret_cast<Vec<U>*>(this);
        }

        /**
         * @brief Get a view over the entire vector.
         */
        [[nodiscard]] View<T> view() const {
            return View<T>(elements_, size_);
        }

     private:
        Vec() = default;

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
                    elements_ = allocator_->reallocate(elements_, new_potential_size);
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

        GenericAllocator* allocator_{ nullptr }; /**< Memory pool where the data is allocated. */
        T*                elements_{ nullptr };  /**< Actual vector elements. */
        usize             size_{ 0U };           /**< Number of elements in the vector. */
        usize             capacity_{ 0U };       /**< Current allocation capacity. */
    };
} // namespace rw
