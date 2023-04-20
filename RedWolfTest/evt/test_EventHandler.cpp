#include <RedWolf/core/Object.hpp>
#include <RedWolf/evt/EventHandler.hpp>
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

struct TestEvt : public rw::evt::Event
{
   TestEvt(size_t i) : Event(5U), increment{ i } {}

   size_t increment{ 0U };
};

struct TestObj : public rw::core::Object
{
   TestObj() : Object() {}

   void userHandle_(const BaseObject* generator, const rw::evt::Event& evt) override
   {
      switch (evt.id)
      {
      case 5U:
         auto& testEvt = dynamic_cast<const TestEvt&>(evt);
         val += testEvt.increment;
         break;
      }
   }

   std::atomic<size_t> val{ 0U };
};

TEST(RwEvtEventHandler, EventGeneration)
{
   rw::util::Logger      logger;
   rw::evt::EventHandler handler{ logger };

   TestObj o1;
   TestObj o2;

   o1.setEventHandler(&handler);
   handler.addObject(&o1);
   o2.setEventHandler(&handler);
   handler.addObject(&o2);

   handler.subscribe(&o1, &o2, 5U);
   o1.generateEvent(TestEvt(1U));
   o1.generateEvent(TestEvt(2U));
   o1.generateEvent(TestEvt(3U));
   o1.generateEvent(TestEvt(4U));
   std::this_thread::sleep_for(std::chrono::seconds(1));
   EXPECT_EQ(o2.val, 10U);

   handler.unsubscribe(&o1, &o2, 5U);
   o1.generateEvent(TestEvt(1U));
   o1.generateEvent(TestEvt(2U));
   o1.generateEvent(TestEvt(3U));
   o1.generateEvent(TestEvt(4U));
   std::this_thread::sleep_for(std::chrono::seconds(1));
   EXPECT_EQ(o2.val, 10U);

   handler.subscribe(&o1, &o2, 5U);
   handler.removeObject(&o2);
   o1.generateEvent(TestEvt(1U));
   o1.generateEvent(TestEvt(2U));
   o1.generateEvent(TestEvt(3U));
   o1.generateEvent(TestEvt(4U));
   std::this_thread::sleep_for(std::chrono::seconds(1));
   EXPECT_EQ(o2.val, 10U);

   handler.removeObject(&o1);
   o1.generateEvent(TestEvt(1U));
   o1.generateEvent(TestEvt(2U));
   o1.generateEvent(TestEvt(3U));
   o1.generateEvent(TestEvt(4U));
   std::this_thread::sleep_for(std::chrono::seconds(1));
   EXPECT_EQ(o2.val, 10U);
}