#pragma once

#include "iterators.hpp"
#include "redwolf/memory/memory.hpp"
#include "view.hpp"

#include <format>
#include <optional>

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
        Str(MemoryCategory memory_category, View<char> text);

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
        void append(View<char> text);

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
         * @brief Get the end iterator of the string. (Poi32s to the null-terminator).
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
         * @brief Get the raw poi32er to the underlying null-terminated string data.
         */
        [[nodiscard]] const char* data() const;
        [[nodiscard]] char*       data();

        /**
         * @brief Check whether the string is empty or not.
         * @return true if the string is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const;

        /**
         * @brief Get the end iterator of the string. (Poi32s to the null-terminator).
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
        [[nodiscard]] std::optional<usize> find_first(View<char>) const;

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
         * @brief Get a subview of the string.
         * @param index Index of the first character of the subview.
         * @param size Number of character of the subview.
         * @return Newly created view.
         */
        [[nodiscard]] View<char> subview(usize index, usize size);

        /**
         * @brief Trim the string from whitespace characters.
         */
        void trim();

        /**
         * @brief Get a view over the string.
         */
        [[nodiscard]] View<char> view() const;

     private:
        Str() = default;

        GenericAllocator* allocator_{ nullptr }; /**< Memory pool where the data is allocated. */
        char*             chars_{ nullptr };     /**< Actual string characters (null-terminated). */
        usize             size_{ 0U };           /**< Number of characters in the string, excluding the null terminator. */
        usize             capacity_{ 0U };       /**< Current allocation capacity. */
    };
} // namespace rw

template<>
struct std::formatter<rw::Str> {
    char  fill_             = ' ';
    char  align_            = '<'; // default left-align for strings
    usize width_            = 0;
    i32   width_arg_id_     = -1;                     // -1 = not dynamic
    usize precision_        = std::string_view::npos; // npos = no truncation
    i32   precision_arg_id_ = -1;

    constexpr auto parse(std::format_parse_context& ctx) {
        auto it  = ctx.begin();
        auto end = ctx.end();

        // fill + align (fill only valid if followed by an align char)
        if (it != end && (it + 1) != end && (*(it + 1) == '<' || *(it + 1) == '>' || *(it + 1) == '^')) {
            fill_  = *it;
            align_ = *(it + 1);
            it += 2;
        } else if (it != end && (*it == '<' || *it == '>' || *it == '^')) {
            align_ = *it;
            ++it;
        }

        // width
        if (it != end && *it == '{') {
            ++it;
            if (it != end && *it == '}') {
                width_arg_id_ = static_cast<i32>(ctx.next_arg_id());
                ++it;
            } else {
                i32 id = 0;
                while (it != end && *it >= '0' && *it <= '9') {
                    id = id * 10 + (*it - '0');
                    ++it;
                }
                ctx.check_arg_id(static_cast<usize>(id));
                width_arg_id_ = id;
                if (it == end || *it != '}') throw std::format_error("invalid width arg-id");
                ++it;
            }
        } else if (it != end && *it >= '0' && *it <= '9') {
            width_ = 0;
            while (it != end && *it >= '0' && *it <= '9') {
                width_ = width_ * 10 + static_cast<usize>(*it - '0');
                ++it;
            }
        }

        // precision
        if (it != end && *it == '.') {
            ++it;
            if (it != end && *it == '{') {
                ++it;
                if (it != end && *it == '}') {
                    precision_arg_id_ = static_cast<i32>(ctx.next_arg_id());
                    ++it;
                } else {
                    i32 id = 0;
                    while (it != end && *it >= '0' && *it <= '9') {
                        id = id * 10 + (*it - '0');
                        ++it;
                    }
                    ctx.check_arg_id(static_cast<usize>(id));
                    precision_arg_id_ = id;
                    if (it == end || *it != '}') throw std::format_error("invalid precision arg-id");
                    ++it;
                }
            } else if (it != end && *it >= '0' && *it <= '9') {
                usize p = 0;
                while (it != end && *it >= '0' && *it <= '9') {
                    p = p * 10 + static_cast<usize>(*it - '0');
                    ++it;
                }
                precision_ = p;
            } else {
                throw std::format_error("expected precision after '.'");
            }
        }

        // optional trailing 's' type char
        if (it != end && *it == 's') ++it;

        if (it != end && *it != '}') throw std::format_error("invalid format spec for rw::Str");

        return it;
    }

    template<class FormatContext>
    auto format(const rw::Str& s, FormatContext& ctx) const -> typename FormatContext::iterator {
        usize width     = width_;
        usize precision = precision_;

        if (width_arg_id_ >= 0) {
            auto arg = ctx.arg(static_cast<usize>(width_arg_id_));
            width    = static_cast<usize>(std::visit_format_arg(
                [](auto v) -> i64 {
                    if constexpr (std::is_integral_v<decltype(v)>)
                        return static_cast<i64>(v);
                    else
                        throw std::format_error("width arg must be integral");
                },
                arg));
        }
        if (precision_arg_id_ >= 0) {
            auto arg  = ctx.arg(static_cast<usize>(precision_arg_id_));
            precision = static_cast<usize>(std::visit_format_arg(
                [](auto v) -> i64 {
                    if constexpr (std::is_integral_v<decltype(v)>)
                        return static_cast<i64>(v);
                    else
                        throw std::format_error("precision arg must be integral");
                },
                arg));
        }

        const char* data = s.data();
        usize       len  = s.size();
        if (precision != std::string_view::npos && precision < len) len = precision;

        auto out = ctx.out();

        if (len >= width) {
            return std::copy_n(data, len, out);
        }

        usize pad      = width - len;
        usize left_pad = 0, right_pad = 0;
        switch (align_) {
        case '<':
            right_pad = pad;
            break;
        case '>':
            left_pad = pad;
            break;
        case '^':
            left_pad  = pad / 2;
            right_pad = pad - left_pad;
            break;
        }

        out = std::fill_n(out, left_pad, fill_);
        out = std::copy_n(data, len, out);
        out = std::fill_n(out, right_pad, fill_);
        return out;
    }
};
