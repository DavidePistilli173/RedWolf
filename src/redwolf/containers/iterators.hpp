#pragma once

#include <iterator>

namespace rw {
    /**
     * @brief ContiguousIterator for the Vec.
     */
    template<typename T, bool IsConst>
    class ContiguousIterator {
     public:
        // --- Required member types for std::contiguous_iterator ---
        using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = std::remove_cv_t<T>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;
        using element_type      = std::conditional_t<IsConst, const T, T>; // required by contiguous_iterator

        // --- Constructors ---
        constexpr ContiguousIterator() noexcept = default;
        constexpr explicit ContiguousIterator(pointer ptr) noexcept : ptr_{ ptr } {}

        // Allow implicit conversion from iterator -> const_iterator
        template<bool WasConst>
            requires(IsConst && !WasConst)
        constexpr ContiguousIterator(const ContiguousIterator<T, WasConst>& other) noexcept : ptr_{ other.ptr_ } {}

        // --- Dereference ---
        constexpr reference operator*() const noexcept {
            return *ptr_;
        }
        constexpr pointer operator->() const noexcept {
            return ptr_;
        }
        constexpr reference operator[](difference_type n) const noexcept {
            return *(ptr_ + n);
        }

        // --- Increment / decrement ---
        constexpr ContiguousIterator& operator++() noexcept {
            ++ptr_;
            return *this;
        }
        constexpr ContiguousIterator operator++(int) noexcept {
            auto result{ ContiguousIterator(ptr_++) };
            return result;
        }
        constexpr ContiguousIterator& operator--() noexcept {
            --ptr_;
            return *this;
        }
        constexpr ContiguousIterator operator--(int) noexcept {
            auto result{ Iteratlr(ptr_--) };
            return result;
        }

        // --- Arithmetic ---
        constexpr ContiguousIterator& operator+=(difference_type n) noexcept {
            ptr_ += n;
            return *this;
        }
        constexpr ContiguousIterator& operator-=(difference_type n) noexcept {
            ptr_ -= n;
            return *this;
        }

        constexpr ContiguousIterator operator+(difference_type n) const noexcept {
            return ContiguousIterator(ptr_ + n);
        }
        constexpr ContiguousIterator operator-(difference_type n) const noexcept {
            return ContiguousIterator(ptr_ - n);
        }

        friend constexpr ContiguousIterator operator+(difference_type n, const ContiguousIterator& it) noexcept {
            return ContiguousIterator(it.ptr_ + n);
        }

        constexpr difference_type operator-(const ContiguousIterator& other) const noexcept {
            return ptr_ - other.ptr_;
        }

        // --- Comparisons (rewritten operators cover ==, !=, <, >, <=, >= via <=>) ---
        constexpr bool                 operator==(const ContiguousIterator& other) const noexcept  = default;
        constexpr std::strong_ordering operator<=>(const ContiguousIterator& other) const noexcept = default;

        // Needed for cross iterator/const_iterator comparison (e.g. it == carr.cend())
        template<bool OtherConst>
        constexpr bool operator==(const ContiguousIterator<T, OtherConst>& other) const noexcept {
            return ptr_ == other.ptr_;
        }
        template<bool OtherConst>
        constexpr std::strong_ordering operator<=>(const ContiguousIterator<T, OtherConst>& other) const noexcept {
            return ptr_ <=> other.ptr_;
        }

     private:
        pointer ptr_{ nullptr };
    };
} // namespace rw