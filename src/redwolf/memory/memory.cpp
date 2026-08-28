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

rw::MemoryPool& rw::Memory::pool(MemoryType type) {
    RW_PROFILE_SCOPE

    switch (type) {
    case MemoryType::events:
        return g_memory->pool_events_;
    case MemoryType::renderer:
        return g_memory->pool_renderer_;
    case MemoryType::engine:
        return g_memory->pool_engine_;
    case MemoryType::modules:
        return g_memory->pool_modules_;
    case MemoryType::app:
        return g_memory->pool_app_;
    case MemoryType::invalid:
    default:
        error("Invalid allocation type: '{}'", static_cast<u8>(type));
        return g_memory->pool_invalid_;
    }
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