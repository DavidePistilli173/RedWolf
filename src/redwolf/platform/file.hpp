#pragma once

#include "redwolf/containers/str.hpp"
#include "redwolf/containers/vec.hpp"

namespace rw {
#ifdef linux
    /**
     * @brief Raw file descriptor.
     */
    using FileDescriptor = i32;

    /**
     * @brief Invalid file descriptor.
     */
    constexpr FileDescriptor invalid_file_descriptor{ -1 };
#endif

    /**
     * @brief Class for accessing files on the file system.
     */
    class File {
     public:
        static constexpr usize buffer_read_size{ 32768 }; /**< Size for buffered reads. [B] */

        /**
         * @brief File open modes.
         */
        enum class OpenMode : u8 {
            read,      /**< Read only. */
            write,     /**< Write only. */
            read_write /**< Both read and write. */
        };

        File(MemoryCategory memory_category, View<char> path);
        ~File();

        File(const File&)            = delete;
        File& operator=(const File&) = delete;

        File(File&& other) noexcept;
        File& operator=(File&& other) noexcept;

        /**
         * @brief Close the file.
         */
        void close();

        /**
         * @brief Check whether the path exists.
         * @return true if the path exists, false otherwise.
         */
        [[nodiscard]] bool exists() const;

        /**
         * @brief Check whether the file is open or not.
         * @return true if the file is open, false otherwise.
         */
        [[nodiscard]] bool is_open() const;

        /**
         * @brief Open the file.
         * @param mode Mode to open the file with.
         * @param binary If true, the file is interpreted as a binary stream of data.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool open(OpenMode mode, bool binary);

        /**
         * @brief Read the raw contents of the file.
         * @param data Data that was read from the file.
         * @param expected_size Size to read. [B]
         * @return true on success (even if not enough bytes are read), false otherwise.
         */
        [[nodiscard]] bool read(Vec<u8>& data, usize expected_size);

        /**
         * @brief Read all the raw contents of the file.
         * @param data Data that was read from the file.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool read_all(Vec<u8>& data);

        /**
         * @brief Read a line of text from the file.
         * @param line Line that was read, trimmed.
         * @return true on success, false otheriwse.
         */
        [[nodiscard]] bool read_line(Str& line);

        /**
         * @brief Write a line of text to the file. Appropriate line endings are added automatically.
         * @param text Text to write.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool write_line(View<char> text);

     private:
        /**
         * @brief Fill the read buffer.
         */
        [[nodiscard]] bool fill_buffer_();

        MemoryCategory memory_category_{ MemoryCategory::invalid }; /**< Memory category used by the file. */
        Str            path_;                                       /**< File path. */
        FileDescriptor fd_{ invalid_file_descriptor };              /**< Raw file descriptor. */
        Vec<u8>        buffer_;                                     /**< Read buffer. */
        usize          buffer_index_{ 0U };                         /**< Index of the next byte to read from the buffer_. */
        bool           eof_{ false };                               /**< If true, EOF has been reached. */
    };
} // namespace rw
