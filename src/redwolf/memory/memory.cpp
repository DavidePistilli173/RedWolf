#include "memory.hpp"

#include "redwolf/logger.hpp"
#include "redwolf/profiler.hpp"

namespace {
    rw::Memory* g_memory{ nullptr }; // Global memory manager instance.
}

rw::GenericAllocator& rw::Memory::allocator(MemoryCategory type) {
    RW_PROFILE_SCOPE

    switch (type) {
    case MemoryCategory::events:
        return g_memory->pool_events_;
    case MemoryCategory::renderer:
        return g_memory->pool_renderer_;
    case MemoryCategory::engine:
        return g_memory->pool_engine_;
    case MemoryCategory::modules:
        return g_memory->pool_modules_;
    case MemoryCategory::app:
        return g_memory->pool_app_;
    case MemoryCategory::invalid:
    default:
        error("Invalid allocation type: '{}'", static_cast<u8>(type));
        return g_memory->pool_invalid_;
    }
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