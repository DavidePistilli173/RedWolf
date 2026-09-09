#pragma once

#include "iterators.hpp"
#include "redwolf/memory/memory.hpp"

#include <optional>
#include <string_view>

namespace rw {
    /**
     * @brief String class.
     */
    class Str {
     public:
        static constexpr usize growth_factor{ 2 }; /**< Memory growth when automatic reallocation is needed. */
        static constexpr usize min_capacity{ 8 };  /**< Minimum number of characters that can be stored. */

        /**
         * @brief Construct an empty string.
         * @param memory_category Type of memory to use.
         */
        explicit Str(MemoryCategory memory_category);

        /**
         * @brief Construct a string with a given starting text.
         * @param memory_category Type of memory to use.
         * @param text Starting text of the string.
         */
        Str(MemoryCategory memory_category, std::string_view text);

        ~Str();

        // Deleted because shallow copies are not allowed.
        // Deep copies are allowed through the clone method to be more explicit.
        Str(const Str&)            = delete;
        Str& operator=(const Str&) = delete;

        Str(Str&& other) noexcept;
        Str& operator=(Str&& other) noexcept;

        /**
         * @brief Subscript operator.
         * @details No check is performed on the index.
         */
        const char& operator[](usize index) const;
        char&       operator[](usize index);

        /**
         * @brief Append some text to the string.
         * @param text Text to append.
         */
        void append(std::string_view text);

        /**
         * @brief Obtain the last character of the string.
         */
        [[nodiscard]] const char& back() const;
        [[nodiscard]] char&       back();

        /**
         * @brief Get an iterator to the first character of the string.
         */
        [[nodiscard]] ContiguousIterator<char, true>  begin() const;
        [[nodiscard]] ContiguousIterator<char, false> begin();

        /**
         * @brief Get the current string capacity.
         */
        [[nodiscard]] usize capacity() const;

        /**
         * @brief Get an iterator to the first character of the string.
         */
        [[nodiscard]] ContiguousIterator<char, true> cbegin() const;

        /**
         * @brief Get the end iterator of the string. (Points to the null-terminator).
         */
        [[nodiscard]] ContiguousIterator<char, true> cend() const;

        /**
         * @brief Clear the contents of the string, without deallocating any memory.
         */
        void clear();

        /**
         * @brief Clone the string.
         */
        [[nodiscard]] Str clone() const;

        /**
         * @brief Get the raw pointer to the underlying null-terminated string data.
         */
        [[nodiscard]] const char* data() const;
        [[nodiscard]] char*       data();

        /**
         * @brief Check whether the string is empty or not.
         * @return true if the string is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const;

        /**
         * @brief Get the end iterator of the string. (Points to the null-terminator).
         */
        [[nodiscard]] ContiguousIterator<char, true>  end() const;
        [[nodiscard]] ContiguousIterator<char, false> end();

        /**
         * @brief Fill the string with a specific character.
         */
        void fill(char value);

        /**
         * @brief Find the first occurence of a given text in the string.
         * @param text Text to find.
         * @return Index inside the string where the text starts, if the text was found.
         */
        [[nodiscard]] std::optional<usize> find_first(std::string_view text) const;

        /**
         * @brief Get a reference to the first character of the string.
         */
        [[nodiscard]] const char& front() const;
        [[nodiscard]] char&       front();

        /**
         * @brief Reset the string, deallocating all its memory.
         */
        void reset();

        /**
         * @brief Reserve a specific size in the string capacity.
         * @param size Number of characters that need to fit in the new capacity (excluding the null terminator).
         */
        void reserve(usize size);

        /**
         * @brief Resize the string to a given size.
         * @param new_size New number of characters of the string. If larger than the current size, new characters are appended. If smaller,
         * the excess elements will be deleted.
         * @param value Value to initialise the new characters with.
         */
        void resize(usize new_size, char value = '\0');

        /**
         * @brief Shrink the string to fit the current number of characters.
         */
        void shrink_to_fit();

        /**
         * @brief Get the number of elements of the string (excluding the null-terminator).
         */
        [[nodiscard]] usize size() const;

        /**
         * @brief Trim the string from whitespace characters.
         */
        void trim();

        /**
         * @brief Get a view over the string.
         */
        [[nodiscard]] std::string_view view() const;

     private:
        Str() = default;

        GenericAllocator* allocator_{ nullptr }; /**< Memory pool where the data is allocated. */
        char*             chars_{ nullptr };     /**< Actual string characters (null-terminated). */
        usize             size_{ 0U };           /**< Number of characters in the string, excluding the null terminator. */
        usize             capacity_{ 0U };       /**< Current allocation capacity. */
    };
} // namespace rw