#pragma once

#include "redwolf/common.hpp"
#include "vec.hpp"

#include <bitset>
#include <chrono>
#include <coroutine>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <stacktrace>
#include <string>
#include <string_view>
#include <system_error>
#include <text_encoding>
#include <thread>
#include <typeindex>
#include <variant>
#include <vector>

namespace rw {
    /**
     * @brief Custom hash map.
     * @tparam KeyT Key type.
     * @tparam ValueT Value type.
     */
    template<typename KeyT, typename ValueT>
    class HashMap {
     public:
        static constexpr usize growth_factor{ 2 }; /**< Memory growth factor on automatic resizing. */
        static constexpr usize min_capacity{ 8 };  /**< Minimum capacity. */

        /**
         * @brief Entry in the map.
         */
        struct Entry {
            KeyT   key;
            ValueT value;
        };

        /**
         * @brief Iterator for HashMap.
         */
        template<bool IsConst>
        class Iterator {
         public:
            // --- Required member types for std::forward_iterator ---
            using iterator_category = std::forward_iterator_tag;
            using value_type        = Entry;
            using difference_type   = std::ptrdiff_t;
            using pointer           = std::conditional_t<IsConst, const Entry*, Entry*>;
            using reference         = std::conditional_t<IsConst, const Entry&, Entry&>;

            using vec_iterator = Vec<std::optional<Entry>>::template Iterator<IsConst>;

            // --- Constructors ---
            constexpr Iterator() noexcept = default;

            // ptr: current slot; end: one-past-last slot of the backing vector,
            // needed so operator++ knows where to stop skipping.
            constexpr Iterator(vec_iterator ptr, vec_iterator end) noexcept : ptr_{ ptr }, end_{ end } {}

            // Allow implicit conversion from iterator -> const_iterator
            template<bool WasConst>
                requires(IsConst && !WasConst)
            constexpr Iterator(const Iterator<WasConst>& other) noexcept : ptr_{ other.ptr }, end_{ other.end } {}

            // --- Dereference (unwraps the optional) ---
            constexpr reference operator*() const noexcept {
                return ptr_->value();
            }
            constexpr pointer operator->() const noexcept {
                return &(ptr_->value());
            }

            // --- Increment (skips slots where has_value() is false) ---
            constexpr Iterator& operator++() noexcept {
                skip_to_valid_();
                return *this;
            }
            constexpr Iterator operator++(int) noexcept {
                auto result{ Iterator(ptr_, end_) };
                skip_to_valid_();
                return result;
            }

            // --- Equality only (forward_iterator does not require <=>) ---
            constexpr bool operator==(const Iterator& other) const noexcept = default;

            template<bool OtherConst>
            constexpr bool operator==(const Iterator<OtherConst>& other) const noexcept {
                return (ptr_ == other.ptr_) && (end_ == other.end_);
            }

         private:
            /**
             * @brief Skip to the next valid pointer, up to the end pointer.
             */
            constexpr void skip_to_valid_() noexcept {
                while ((ptr_ != end_) && (!ptr_->has_value())) {
                    ++ptr_;
                }
            }

            Vec<std::optional<Entry>>::template Iterator<IsConst> ptr_;
            Vec<std::optional<Entry>>::template Iterator<IsConst> end_; // bound for skip-scanning
        };

        explicit HashMap(MemoryType memory_type) : memory_type_{ memory_type }, table_{ memory_type } {}

        /**
         * @brief Get an element from the map.
         * @param key Key to look for.
         * @return Reference to the value matching the given key. If the value was not present, a new one is inserted.
         */
        [[nodiscard]] ValueT& operator[](const KeyT& key) {
            const usize index{ find_(key) };

            // Element found.
            if (table_[index].has_value()) {
                return table_[index].value;
            }

            // Insert a default constructed element.
            return insert(key, ValueT{});
        }

        /**
         * @brief Return an iterator to the first valid element.
         */
        [[nodiscard]] Iterator<false> begin() {
            auto result{ Iterator(table_.begin(), table_.end()) };
            result.skip_to_valid_();
            return result;
        }

        /**
         * @brief Return an iterator to the first valid element.
         */
        [[nodiscard]] Iterator<true> begin() const {
            auto result{ Iterator(table_.begin(), table_.end()) };
            result.skip_to_valid_();
            return result;
        }

        /**
         * @brief Return an iterator to the first valid element.
         */
        [[nodiscard]] Iterator<true> cbegin() const {
            auto result{ Iterator(table_.begin(), table_.end()) };
            result.skip_to_valid_();
            return result;
        }

        /**
         * @brief Get an iterator to the first element after the last valid one.
         */
        [[nodiscard]] Iterator<true> cend() const {
            return Iterator(table_.end(), table_.end());
        }

        /**
         * @brief Clear all elements from the map. Does not deallocate any memory.
         */
        void clear() {
            table_.clear();
            size_ = 0;
        }

        /**
         * @brief Check whether the map contains an element or not.
         * @param key Key to check.
         * @return true if the element is present in the map, false otherwise.
         */
        [[nodiscard]] bool contains(const KeyT& key) {
            return find_(key).has_value();
        }

        /**
         * @brief Construct a new element in place.
         * @param key Key of the new element.
         * @param args Arguments for constructing the new value.
         */
        template<typename... Args>
        [[nodiscard]] ValueT& emplace(const KeyT& key, Args&&... args) {
            if (size_ >= (table_.size() / 2)) {
                resize_();
            }

            const usize index{ find_(key) };

            // New key.
            if (!table_[index].has_value()) {
                table_[index].emplace(Entry{ .key = key, .value = ValueT(std::forward<Args>(args)...) });
                ++size_;
                return table_[index]->value;
            }

            // Key already present.
            table_[index]->value = ValueT(std::forward<Args>(args)...);
            return table_[index]->value;
        }

        /**
         * @brief Check whether the map is empty or not.
         * @return true if the map is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const {
            return 0 == size_;
        }

        /**
         * @brief Get an iterator to the first element after the last valid one.
         */
        [[nodiscard]] Iterator<false> end() {
            return Iterator<false>(table_.end(), table_.end());
        }

        /**
         * @brief Get an iterator to the first element after the last valid one.
         */
        [[nodiscard]] Iterator<true> end() const {
            return Iterator<true>(table_.end(), table_.end());
        }

        /**
         * @brief Erase an item from the map.
         * @param key Key to erase. If the key does not exist, nothing happens.
         */
        void erase(const KeyT& key) {
            usize index{ find_(key) };
            if (table_[index].has_value()) {
                // Find the last element of this valid stride and move it here.
                usize last_index{ (index + 1) % table_.size() };
                while (table_[index].has_value()) {
                    index = (index + 1) & table_.size();
                }

                table_[index] = std::move(table_[last_index]);
                --size_;
            }
        }

        /**
         * @brief Find the element with a given key.
         * @param key Key to find.
         * @return Iterator to the element, or the end iterator.
         */
        [[nodiscard]] Iterator<false> find(const KeyT& key) {
            if (0 == size_) {
                return Iterator<false>(table_.end(), table_.end());
            }

            const usize index{ find_(key) };
            if (table_[index].has_value()) {
                return Iterator<false>(table_.begin() + index, table_.end());
            }
            return Iterator<false>(table_.end(), table_.end());
        }

        /**
         * @brief Find the element with a given key.
         * @param key Key to find.
         * @return Iterator to the element, or the end iterator.
         */
        [[nodiscard]] Iterator<true> find(const KeyT& key) const {
            const usize index{ find_(key) };
            if (table_[index].has_value()) {
                return Iterator<true>(table_.begin() + index, table_.end());
            }
            return Iterator<true>(table_.end(), table_.end());
        }

        /**
         * @brief Insert a new element into the map.
         * @param key Element key.
         * @param value Element value.
         */
        [[nodiscard]] ValueT& insert(const KeyT& key, const ValueT& value) {
            if (size_ >= (table_.size() / 2)) {
                resize_();
            }

            const usize index{ find_(key) };

            // New key.
            if (!table_[index].has_value()) {
                table_[index].emplace(Entry{ .key = key, .value = value });
                ++size_;
                return table_[index].value;
            }

            // Key already present.
            table_[index].value = value;
            return table_[index].value;
        }

        /**
         * @brief Get the number of elements in the map.
         */
        [[nodiscard]] usize size() const {
            return size_;
        }

     private:
        /**
         * @brief Find an element with a specific key.
         * @param key Key to find.
         * @return Index of the desired element, or of the appropriate empty slot to insert it into.
         */
        [[nodiscard]] usize find_(const KeyT& key) {
            usize idx{ hash_(key, table_.size()) };
            while (table_[idx].has_value()) {
                if (table_[idx]->key == key) {
                    return idx;
                }
                idx = (idx + 1) % table_.size();
            }
            return idx;
        }

        /**
         * @brief Obtain a key's hash.
         * @param key Key to hash.
         * @param table_capacity Capacity of the map table.
         * @return Key's hash, usable as index in the table vector.
         */
        [[nodiscard]] static usize hash_(const KeyT& key, usize table_capacity) {
            return std::hash<KeyT>{}(key) % table_capacity;
        }

        /**
         * @brief Resize the map, making space for more entries.
         */
        void resize_() {
            const usize               new_capacity{ std::max(min_capacity, table_.size() * growth_factor) };
            Vec<std::optional<Entry>> new_table{ memory_type_ };
            new_table.resize(new_capacity);

            for (auto& entry : table_) {
                if (entry.has_value()) {
                    usize idx{ hash_(entry->key, new_capacity) };
                    while (new_table[idx].has_value()) {
                        idx = (idx + 1) % new_capacity;
                    }
                    new_table[idx] = std::move(entry);
                }
            }

            table_ = std::move(new_table);
        }

        MemoryType                memory_type_{ MemoryType::engine }; /**< Memory pool used by the hashmap. */
        Vec<std::optional<Entry>> table_{ MemoryType::engine };       /**< Actual map data. */
        usize                     size_{ 0U };                        /**< Number of real elements stored in the map. */
    };
} // namespace rw