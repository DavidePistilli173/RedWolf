#include <RedWolf/evt/Event.hpp>
#include <gtest/gtest.h>

TEST(RwEvtEvent, Constructor)
{
   rw::evt::Event e1{ 55U };
   EXPECT_EQ(e1.id, 55U);
}