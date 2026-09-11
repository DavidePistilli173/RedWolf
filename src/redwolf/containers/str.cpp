#include "str.hpp"

#include <cstring>

rw::Str::Str(MemoryCategory memory_category) : allocator_{ &Memory::allocator(memory_category) } {}

rw::Str::Str(MemoryCategory memory_category, View<char> text) :
    allocator_{ &Memory::allocator(memory_category) }, size_{ text.size() }, capacity_{ text.size() + 1 } {
    chars_ = allocator_->reallocate(chars_, capacity_);
    std::memcpy(chars_, text.data(), size_);
    chars_[size_] = '\0';
}

rw::Str::~Str() {
    reset();
}

rw::Str::Str(Str&& other) noexcept :
    allocator_{ other.allocator_ }, chars_{ other.chars_ }, size_{ other.size_ }, capacity_{ other.capacity_ } {
    other.chars_    = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
}

rw::Str& rw::Str::operator=(Str&& other) noexcept {
    if (this != &other) {
        reset();

        allocator_ = other.allocator_;

        chars_       = other.chars_;
        other.chars_ = nullptr;

        size_       = other.size_;
        other.size_ = 0;

        capacity_       = other.capacity_;
        other.capacity_ = 0;
    }
    return *this;
}

const char& rw::Str::operator[](usize index) const {
    return chars_[index];
}

char& rw::Str::operator[](usize index) {
    return chars_[index];
}

void rw::Str::append(View<char> text) {
    reserve(size_ + text.size() + 1);
    std::memcpy(&chars_[size_], text.data(), text.size());
    size_ += text.size();
    chars_[size_] = '\0';
}

const char& rw::Str::back() const {
    return chars_[size_ - 1];
}

char& rw::Str::back() {
    return chars_[size_ - 1];
}

rw::ContiguousIterator<char, true> rw::Str::begin() const {
    return ContiguousIterator<char, true>(chars_);
}

rw::ContiguousIterator<char, false> rw::Str::begin() {
    return ContiguousIterator<char, false>(chars_);
}

usize rw::Str::capacity() const {
    return capacity_;
}

rw::ContiguousIterator<char, true> rw::Str::cbegin() const {
    return ContiguousIterator<char, true>(chars_);
}

rw::ContiguousIterator<char, true> rw::Str::cend() const {
    return ContiguousIterator<char, true>(chars_ + size_);
}

void rw::Str::clear() {
    // If the string is empty, do nothing.
    if (0 == size_) {
        return;
    }

    size_     = 0;
    chars_[0] = '\0';
}

rw::Str rw::Str::clone() const {
    rw::Str new_str;
    new_str.allocator_ = allocator_;
    new_str.reserve(size_ + 1);
    std::memcpy(new_str.chars_, chars_, size_);
    new_str.chars_[new_str.size_] = '\0';
    new_str.size_                 = size_;
    return new_str;
}

const char* rw::Str::data() const {
    return chars_;
}

char* rw::Str::data() {
    return chars_;
}

bool rw::Str::empty() const {
    return 0 == size_;
}

rw::ContiguousIterator<char, true> rw::Str::end() const {
    return ContiguousIterator<char, true>(chars_ + size_);
}

rw::ContiguousIterator<char, false> rw::Str::end() {
    return ContiguousIterator<char, false>(chars_ + size_);
}

void rw::Str::fill(char value) {
    std::memset(chars_, static_cast<int32_t>(value), size_);
}

std::optional<usize> rw::Str::find_first(View<char> text) const {
    // The text cannot be inside the string.
    if (text.size() > size_) {
        return {};
    }

    bool  found{ false };
    usize result{ 0U };
    usize text_index{ 0U };
    usize str_index{ 0U };
    while (!found && (size_ - str_index) >= (text.size() - text_index)) {
        if (chars_[str_index] == text[text_index]) {
            // If this is the first character, mark it as a possible result.
            if (0 == text_index) {
                result = str_index;
            }

            ++text_index;
            if (text_index == text.size()) {
                found = true;
            }
        }

        ++str_index;
    }

    if (!found) {
        return {};
    }
    return result;
}

const char& rw::Str::front() const {
    return chars_[0];
}

char& rw::Str::front() {
    return chars_[0];
}

void rw::Str::reset() {
    if (nullptr != chars_) {
        clear();
        allocator_->deallocate(chars_);
        chars_ = nullptr;
    }
}

void rw::Str::reserve(usize size) {
    // String already large enough: nothing to do.
    if (capacity_ >= size) {
        return;
    }

    chars_    = allocator_->reallocate(chars_, size);
    capacity_ = size;
}

void rw::Str::resize(usize new_size, char value) {
    // Same size, nothing to do.
    if (new_size == size_) {
        return;
    }

    // Shrink the string.
    if (new_size < size_) {
        size_         = new_size;
        chars_[size_] = '\0';
        return;
    }

    // Expand the string.
    if (new_size > capacity_) {
        chars_    = allocator_->reallocate(chars_, new_size + 1);
        capacity_ = new_size + 1;
    }
    std::memset(&chars_[size_], static_cast<int32_t>(value), new_size - size_);
    size_         = new_size;
    chars_[size_] = '\0';
}

void rw::Str::shrink_to_fit() {
    // String already small enough: nothing to do.
    if (capacity_ == size_) {
        return;
    }

    chars_    = allocator_->reallocate(chars_, size_);
    capacity_ = size_;
}

usize rw::Str::size() const {
    return size_;
}

rw::View<char> rw::Str::subview(usize index, usize size) {
    return View<char>(chars_ + index, size);
}

void rw::Str::trim() {
    // If the string is empty, it is already trimmed.
    if (0 == size_) {
        return;
    }

    // Trim the back of the string.
    usize index{ size_ - 1 };
    while ((0 < index) && std::isspace(chars_[index])) {
        --index;
    }
    size_         = index + 1;
    chars_[size_] = '\0';

    // Trim the front of the string.
    index = 0;
    while ((index < size_) && std::isspace(chars_[index])) {
        ++index;
    }
    size_ -= index;
    if (0 != index) {
        std::memmove(chars_, chars_ + index, size_);
        chars_[size_] = '\0';
    }
}

rw::View<char> rw::Str::view() const {
    return { chars_, size_ };
}
