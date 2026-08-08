#include "memory.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/profiler.hpp"

namespace {
    rw::Memory* g_memory{ nullptr }; // Global memory manager instance.
}

bool rw::Memory::init() {
    RW_PROFILE_SCOPE

    if (nullptr != g_memory) {
        warn("Memory manager already initialised.");
        return true;
    }

    g_memory = new Memory();
    return true;
}
