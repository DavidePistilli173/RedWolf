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

void rw::Memory::shutdown() {
    RW_PROFILE_SCOPE

    if (nullptr == g_memory) {
        warn("De-initialising memory sustem twice.");
        return;
    }

    delete g_memory;
    g_memory = nullptr;
}

rw::Memory* rw::Memory::instance_() {
    return g_memory;
}