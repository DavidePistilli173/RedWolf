#include "gtest/gtest.h"

import rw.core.aliases;
import rw.core.Component;

TEST(RwCoreComponent, Constructor)
{
   static constexpr rw::core::ComponentID id1{ 5U };

   rw::core::Component t1(id1);

   EXPECT_EQ(t1.id, id1);
}