#include "file.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

rw::File::File(MemoryCategory memory_category, View<char> path) :
    memory_category_{ memory_category }, path_{ memory_category, path }, buffer_{ memory_category } {}

rw::File::~File() {
    close();
}

rw::File::File(File&& other) noexcept :
    memory_category_{ other.memory_category_ }, path_{ std::move(other.path_) }, fd_{ other.fd_ }, buffer_{ std::move(other.buffer_) },
    buffer_index_{ other.buffer_index_ }, eof_{ other.eof_ } {
    other.fd_ = invalid_file_descriptor;
}

rw::File& rw::File::operator=(File&& other) noexcept {
    if (this != &other) {
        memory_category_ = other.memory_category_;
        path_            = std::move(other.path_);

        fd_       = other.fd_;
        other.fd_ = invalid_file_descriptor;

        buffer_       = std::move(other.buffer_);
        buffer_index_ = other.buffer_index_;
        eof_          = other.eof_;
    }
    return *this;
}

void rw::File::close() {
    if (invalid_file_descriptor != fd_) {
        ::close(fd_);
    }
}

bool rw::File::exists() const {
    struct stat buffer{};
    return 0 == stat(path_.data(), &buffer);
}

bool rw::File::is_open() const {
    return invalid_file_descriptor != fd_;
}

// In linux, binary mode is the only mode.
bool rw::File::open(OpenMode mode, [[maybe_unused]] bool binary) {
    // Prepare the flags
    i32 flags{ 0 };

    switch (mode) {
    case OpenMode::read:
        flags |= O_RDONLY;
        break;
    case OpenMode::write:
        flags |= O_CREAT;
        flags |= O_TRUNC;
        flags |= O_WRONLY;
        break;
    case OpenMode::read_write:
        flags |= O_CREAT;
        flags |= O_RDWR;
        break;
    default:
        error("Unknown file open mode: '{}'", static_cast<u8>(mode));
        return false;
    }

    constexpr i32 permissions{ 0644 };

    // Attempt to open the file.
    fd_ = ::open(path_.data(), flags, permissions);
    if (invalid_file_descriptor == fd_) {
        error("Failed to open file '{}'", path_);
        return false;
    }

    return true;
}

bool rw::File::read(Vec<u8>& data, usize expected_size) {
    if (invalid_file_descriptor == fd_) {
        warn("File '{}' not open.", path_);
        return false;
    }

    while ((data.size() < expected_size) && (!eof_ || !buffer_.empty())) {
        // If the read buffer is empty, fill it.
        if (buffer_.empty() && !fill_buffer_()) {
            warn("Failed to fill the read buffer for file '{}'", path_);
            return false;
        }

        const usize remaining_size{ expected_size - data.size() };
        const usize size_to_append{ std::min(remaining_size, buffer_.size()) };
        data.append(buffer_.subview(0, size_to_append));
        buffer_index_ += size_to_append;

        // If the buffer is complete, clear it.
        if (buffer_index_ == buffer_.size()) {
            buffer_.clear();
        }
    }

    return true;
}

bool rw::File::read_all(Vec<u8>& data) {
    if (invalid_file_descriptor == fd_) {
        warn("File '{}' not open.", path_);
        return false;
    }

    while (!eof_ || !buffer_.empty()) {
        // If the read buffer is empty, fill it.
        if (buffer_.empty() && !fill_buffer_()) {
            warn("Failed to fill the read buffer for file '{}'", path_);
            return false;
        }

        data.append(buffer_);
        buffer_.clear();
    }

    return true;
}

bool rw::File::read_line(Str& line) {
    if (invalid_file_descriptor == fd_) {
        warn("File '{}' not open.", path_);
        return false;
    }

    // Iterate until a newline has been found, or EOF has been reached and the buffer is empty.
    bool newline_found{ false };
    while (!newline_found && (!eof_ || !buffer_.empty())) {
        // If the read buffer is empty, fill it.
        if (buffer_.empty() && !fill_buffer_()) {
            warn("Failed to fill the read buffer for file '{}'", path_);
            return false;
        }

        // Find the first newline in the buffer and append to the output string.
        const auto newline_res{ buffer_.find_first([](const char& elem) { return '\n' == elem; }) };
        if (!newline_res.has_value()) {
            line.append(buffer_.subview(buffer_index_, buffer_.size() - buffer_index_).to<char>());
        } else {
            line.append(buffer_.subview(buffer_index_, buffer_.size() - newline_res.value()).to<char>());
            buffer_index_ += newline_res.value();
            newline_found = true;
        }

        // If the buffer is complete, clear it.
        if (buffer_index_ == buffer_.size()) {
            buffer_.clear();
        }
    }

    line.trim();
    return true;
}

bool rw::File::write_line(View<char> text) {
    if (invalid_file_descriptor == fd_) {
        warn("File '{}' not open.", path_);
        return false;
    }

    Str output_text{ memory_category_, text };
    output_text.append("\n");

    const isize written_bytes{ ::write(fd_, output_text.data(), output_text.size()) };
    if (text.size() > written_bytes) {
        warn("Failed to write to file '{}'", path_);
        return false;
    }

    return true;
}

bool rw::File::fill_buffer_() {
    buffer_.resize(buffer_read_size);
    const isize bytes_read{ ::read(fd_, buffer_.data(), buffer_read_size) };
    if (-1 == bytes_read) {
        warn("Failed to read from file '{}'", path_);
        return false;
    }

    if (buffer_read_size > bytes_read) {
        eof_ = true;
    }

    buffer_.resize(static_cast<usize>(bytes_read));
    buffer_index_ = 0U;
    return true;
}
