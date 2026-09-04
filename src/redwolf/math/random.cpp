#include "random.hpp"

#include "redwolf/logger.hpp"

#include <cmath>

namespace {
    rw::Random* g_random{ nullptr };
}

void rw::Random::init(u64 seed) {
    if (nullptr != g_random) {
        warn("Random number generator already initialised.");
        return;
    }
    g_random = new Random();

    u64 seed_state{ seed };

    for (auto& s : g_random->state_) {
        s = next_seed_(seed_state);
    }
}

f32 rw::Random::rand_f32() {
    return static_cast<f32>(rand_u32() >> 8) * (1.0f / 16777216.0f);
}

f32 rw::Random::rand_f32_in_range(f32 min, f32 max) {
    return std::lerp(min, max, rand_f32());
}

f64 rw::Random::rand_f64() {
    return static_cast<f64>(rand_u64() >> 11) * (1.0 / 9007199254740992.0);
}

i32 rw::Random::rand_i32_in_range(i32 min, i32 max) {
    const u32 range{ static_cast<u32>(max) - static_cast<u32>(min) + 1U };
    return static_cast<i32>(static_cast<u32>(min) + rand_u32_in_range(0, range));
}

u32 rw::Random::rand_u32() {
    if (g_random->has_cached_u32_) {
        g_random->has_cached_u32_ = false;
        return static_cast<u32>(g_random->cached_u64_ >> 32);
    }
    g_random->cached_u64_     = rand_u64();
    g_random->has_cached_u32_ = true;
    return static_cast<u32>(g_random->cached_u64_);
}

u32 rw::Random::rand_u32_in_range(u32 min, u32 max) {
    u64 product = static_cast<u64>(static_cast<u32>(rand_u64())) * max;
    u32 low     = static_cast<u32>(product);
    if (low < max) {
        const u32 threshold = (0u - max) % max;
        while (low < threshold) {
            product = static_cast<u64>(static_cast<u32>(rand_u64())) * max;
            low     = static_cast<u32>(product);
        }
    }
    return static_cast<u32>(product >> 32) + min;
}

u64 rw::Random::rand_u64() {
    auto& state{ g_random->state_ };

    const u64 result = rotl_(state[1] * 5, 7) * 9;

    const u64 t = state[1] << 17;

    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];

    state[2] ^= t;

    state[3] = rotl_(state[3], 45);

    return result;
}

void rw::Random::shutdown() {
    if (nullptr == g_random) {
        warn("Random number generator already shut down.");
        return;
    }

    delete g_random;
}

u64 rw::Random::next_seed_(u64& state) {
    u64 z = (state += 0x9E3779B97F4A7C15ULL);
    z     = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z     = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

u64 rw::Random::rotl_(u64 x, int k) {
    return (x << k) | (x >> (64 - k));
}