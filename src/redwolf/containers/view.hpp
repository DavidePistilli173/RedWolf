#pragma once

#include "iterators.hpp"
#include "redwolf/common.hpp"

#include <cstring>
#include <format>
#include <optional>
#include <string_view>
#include <type_traits>

namespace rw {
    /**
     * @brief Generic view over some contiguous non-modifiable data.
     */
    template<typename T>
    class View {
     public:
        static constexpr std::array<u8, sizeof(T)> null_element{};

        constexpr View() = default;

        /**
         * @brief Construct a view from a null-terminated array of elements.
         */
        constexpr View(const T* data) : elements_{ data } {
            const T* current_element{ elements_ };

            if constexpr (std::is_fundamental_v<T>) {
                while (0 != *current_element) {
                    ++current_element;
                }
            } else {
                while (0 != std::memcmp(null_element.data(), current_element, sizeof(T))) {
                    ++current_element;
                }
            }
            size_ = current_element - elements_;
        }

        constexpr View(const T* data, usize size) : elements_{ data }, size_{ size } {}

        /**
         * @brief Subscript operator.
         * @details No check is performed on the index.
         */
        constexpr const T& operator[](usize index) const {
            return elements_[index];
        }

        /**
         * @brief Get the last element of the view.
         */
        [[nodiscard]] constexpr const T& back() const {
            return elements_[size_ - 1];
        }

        /**
         * @brief Get an iterator to the first element of the vector.
         */
        [[nodiscard]] constexpr ContiguousIterator<T, true> begin() const {
            return ContiguousIterator<T, true>(elements_);
        }

        /**
         * @brief Get an iterator to the first element of the vector.
         */
        [[nodiscard]] constexpr ContiguousIterator<T, true> cbegin() const {
            return ContiguousIterator<T, true>(elements_);
        }

        /**
         * @brief Get the end iterator of the view. (One item past the last valid item)
         */
        [[nodiscard]] constexpr ContiguousIterator<T, true> cend() const {
            return ContiguousIterator<T, true>(elements_ + size_);
        }

        /**
         * @brief Get the raw pointer to the data.
         */
        [[nodiscard]] constexpr const T* data() const {
            return elements_;
        }

        /**
         * @brief Check whether the view is empty or not.
         * @return true if the view is empty, false otherwise.
         */
        [[nodiscard]] constexpr bool empty() const {
            return 0 == size_;
        }

        /**
         * @brief Get the end iterator of the view. (One item past the last valid item)
         */
        [[nodiscard]] constexpr ContiguousIterator<T, true> end() const {
            return ContiguousIterator<T, true>(elements_ + size_);
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
        [[nodiscard]] constexpr std::optional<usize> find_first(FnT fn) {
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
        [[nodiscard]] constexpr const T& front() const {
            return elements_[0];
        }

        /**
         * @brief Get the number of elements in the view.
         */
        [[nodiscard]] constexpr usize size() const {
            return size_;
        }

        /**
         * @brief Obtain a subview from the current view.
         * @param index Index of the first element of the subview.
         * @param size Number of elements of the subview.
         * @return Newly created view.
         */
        [[nodiscard]] constexpr View subview(usize index, usize size) const {
            return View(elements_ + index, size);
        }

        /**
         * @brief Cast the view to have another underlying type.
         */
        template<typename U>
        [[nodiscard]] View<U> to() const {
            return View<U>(reinterpret_cast<const U*>(elements_), size_);
        }

     private:
        const T* elements_{ nullptr }; /**< First element of the view. */
        usize    size_{ 0U };          /**< Number of elements. */
    };
} // namespace rw

template<>
struct std::formatter<rw::View<char>> : std::formatter<std::string_view> {
    auto format(const rw::View<char>& v, auto& ctx) const {
        return std::formatter<std::string_view>::format(std::string_view{ v.data(), v.size() }, ctx);
    }
};
