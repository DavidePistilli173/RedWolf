#include "RedWolf/utility.hpp"

#include <gtest/gtest.h>
#include <RedWolf/gl/Buffer.hpp>

constexpr size_t fqt_max_queue_size{ 1024 };
constexpr size_t fqt_max_iterations{ 2 * fqt_max_queue_size };

TEST(FixedQueue, SizeAfterPush)
{
    /* Create a queue. */
    rw::FixedQueue<int, fqt_max_queue_size> q;

    /* Push more elements than the queue can hold. */
    for (int i = 0; i < fqt_max_iterations; ++i)
    {
        q.push(i);
    }
    ASSERT_EQ(q.size(), fqt_max_queue_size);
}