#include "linear_allocator.hpp"

rw::LinearAllocator::LinearAllocator(usize total_size) : total_size_{ total_size } {
    memory_ = new u8[total_size];
}

rw::LinearAllocator::~LinearAllocator() {
    deallocate_all();
    delete[] memory_;
}

void rw::LinearAllocator::deallocate_all() {
    allocated_size_ = 0U;
}