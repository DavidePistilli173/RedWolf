#include "file.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

rw::File::File(MemoryCategory memory_category, std::string_view path) : path_{ memory_category, path }, buffer_{ memory_category } {}

rw::File::~File() {
    close();
}

rw::File::File(File&& other) noexcept : path_{ std::move(other.path_) }, fd_{ other.fd_ }, buffer_{ std::move(other.buffer_) } {
    other.fd_ = invalid_file_descriptor;
}

rw::File& rw::File::operator=(File&& other) noexcept {
    if (this != &other) {
        path_ = std::move(other.path_);

        fd_       = other.fd_;
        other.fd_ = invalid_file_descriptor;

        buffer_ = std::move(other.buffer_);
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

bool rw::File::read_line(Str& line) {
    if (invalid_file_descriptor == fd_) {
        warn("File '{}' not open.", path_);
        return false;
    }

    line.clear();
    if (buffer_.empty()) {
        buffer_.resize(usize new_size) const isize bytes_read {}
    }

    return true;
}