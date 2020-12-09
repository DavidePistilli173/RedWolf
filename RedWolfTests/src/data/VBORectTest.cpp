#include <RedWolf/data/VBORect.hpp>
#include <gtest/gtest.h>

namespace data_vborect_test
{
   constexpr float rounding_error{ 10e-4 };
   constexpr float z{ 5.0F };

   constexpr rw::Rect<float> rect1{ 1.0F, 2.0F, 5.0F, 10.0F };
   constexpr rw::Rect<float> rect2{ 1.0F, 2.0F, -5.0F, 10.0F };
   constexpr rw::Rect<float> rect3{ 1.0F, 2.0F, 5.0F, -10.0F };
   constexpr rw::Rect<float> rect4{ 1.0F, 2.0F, -5.0F, -10.0F };

   constexpr rw::Rect<float> texRect1{ 0.2F, 0.4F, 0.8F, 1.0F };
   constexpr rw::Rect<float> texRect2{ 0.2F, 0.4F, -0.8F, 1.0F };
   constexpr rw::Rect<float> texRect3{ 0.2F, 0.4F, 0.8F, -1.0F };
   constexpr rw::Rect<float> texRect4{ 0.2F, 0.4F, -0.8F, -1.0F };

   constexpr std::array<float, rw::data::VBORect::size> defaultData{
      // Top-left corner
      0.0F,
      0.0F,
      0.0F,
      0.0F,
      0.0F,
      // Top-right corner
      0.0F,
      0.0F,
      0.0F,
      1.0F,
      0.0F,
      // Bottom-right corner
      0.0F,
      0.0F,
      0.0F,
      1.0F,
      1.0F,
      // Bottom-left corner
      0.0F,
      0.0F,
      0.0F,
      0.0F,
      1.0F
   };

   constexpr std::array<float, rw::data::VBORect::size> data_1_0{
      // Top-left corner
      1.0F,
      2.0F,
      0.0F,
      0.0F,
      0.0F,
      // Top-right corner
      6.0F,
      2.0F,
      0.0F,
      1.0F,
      0.0F,
      // Bottom-right corner
      6.0F,
      12.0F,
      0.0F,
      1.0F,
      1.0F,
      // Bottom-left corner
      1.0F,
      12.0F,
      0.0F,
      0.0F,
      1.0F
   };

   constexpr std::array<float, rw::data::VBORect::size> data_1_4{
      // Top-left corner
      1.0F,
      2.0F,
      0.0F,
      0.2F,
      0.4F,
      // Top-right corner
      6.0F,
      2.0F,
      0.0F,
      -0.6F,
      0.4F,
      // Bottom-right corner
      6.0F,
      12.0F,
      0.0F,
      -0.6F,
      -0.6F,
      // Bottom-left corner
      1.0F,
      12.0F,
      0.0F,
      0.2F,
      -0.6F
   };

   constexpr std::array<float, rw::data::VBORect::size> data_2_3{
      // Top-left corner
      1.0F,
      2.0F,
      0.0F,
      0.2F,
      0.4F,
      // Top-right corner
      -4.0F,
      2.0F,
      0.0F,
      1.0F,
      0.4F,
      // Bottom-right corner
      -4.0F,
      12.0F,
      0.0F,
      1.0F,
      -0.6F,
      // Bottom-left corner
      1.0F,
      12.0F,
      0.0F,
      0.2F,
      -0.6F
   };

   constexpr std::array<float, rw::data::VBORect::size> data_3_2{
      // Top-left corner
      1.0F,
      2.0F,
      0.0F,
      0.2F,
      0.4F,
      // Top-right corner
      6.0F,
      2.0F,
      0.0F,
      -0.6F,
      0.4F,
      // Bottom-right corner
      6.0F,
      -8.0F,
      0.0F,
      -0.6F,
      1.4F,
      // Bottom-left corner
      1.0F,
      -8.0F,
      0.0F,
      0.2F,
      1.4F
   };

   constexpr std::array<float, rw::data::VBORect::size> data_4_1{
      // Top-left corner
      1.0F,
      2.0F,
      0.0F,
      0.2F,
      0.4F,
      // Top-right corner
      -4.0F,
      2.0F,
      0.0F,
      1.0F,
      0.4F,
      // Bottom-right corner
      -4.0F,
      -8.0F,
      0.0F,
      1.0F,
      1.4F,
      // Bottom-left corner
      1.0F,
      -8.0F,
      0.0F,
      0.2F,
      1.4F
   };

   void testEqToOriginal_VtxX(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      using VtxIdx = rw::data::VBORect::VtxIdx;
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tl_x)],
         originalData[static_cast<size_t>(VtxIdx::tl_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tr_x)],
         originalData[static_cast<size_t>(VtxIdx::tr_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::br_x)],
         originalData[static_cast<size_t>(VtxIdx::br_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::bl_x)],
         originalData[static_cast<size_t>(VtxIdx::bl_x)],
         rounding_error);
   }

   void testEqToOriginal_VtxY(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      using VtxIdx = rw::data::VBORect::VtxIdx;
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tl_y)],
         originalData[static_cast<size_t>(VtxIdx::tl_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tr_y)],
         originalData[static_cast<size_t>(VtxIdx::tr_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::br_y)],
         originalData[static_cast<size_t>(VtxIdx::br_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::bl_y)],
         originalData[static_cast<size_t>(VtxIdx::bl_y)],
         rounding_error);
   }

   void testEqToOriginal_VtxZ(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      using VtxIdx = rw::data::VBORect::VtxIdx;
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tl_z)],
         originalData[static_cast<size_t>(VtxIdx::tl_z)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::tr_z)],
         originalData[static_cast<size_t>(VtxIdx::tr_z)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::br_z)],
         originalData[static_cast<size_t>(VtxIdx::br_z)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(VtxIdx::bl_z)],
         originalData[static_cast<size_t>(VtxIdx::bl_z)],
         rounding_error);
   }

   void testEqToOriginal_TexX(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      using TexIdx = rw::data::VBORect::TexIdx;
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::tl_x)],
         originalData[static_cast<size_t>(TexIdx::tl_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::tr_x)],
         originalData[static_cast<size_t>(TexIdx::tr_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::br_x)],
         originalData[static_cast<size_t>(TexIdx::br_x)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::bl_x)],
         originalData[static_cast<size_t>(TexIdx::bl_x)],
         rounding_error);
   }

   void testEqToOriginal_TexY(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      using TexIdx = rw::data::VBORect::TexIdx;
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::tl_y)],
         originalData[static_cast<size_t>(TexIdx::tl_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::tr_y)],
         originalData[static_cast<size_t>(TexIdx::tr_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::br_y)],
         originalData[static_cast<size_t>(TexIdx::br_y)],
         rounding_error);
      ASSERT_NEAR(
         data[static_cast<size_t>(TexIdx::bl_y)],
         originalData[static_cast<size_t>(TexIdx::bl_y)],
         rounding_error);
   }

   void testEqToOriginal_VtxAll(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      testEqToOriginal_VtxX(data, originalData);
      testEqToOriginal_VtxY(data, originalData);
      testEqToOriginal_VtxZ(data, originalData);
   }

   void testEqToOriginal_TexAll(
      const std::array<float, rw::data::VBORect::size>& data,
      const std::array<float, rw::data::VBORect::size>& originalData)
   {
      testEqToOriginal_TexX(data, originalData);
      testEqToOriginal_TexY(data, originalData);
   }

   void testRect(rw::data::VBORect& vborect, const rw::Rect<float>& rect, const rw::Rect<float>& texRect)
   {
      const std::array<float, rw::data::VBORect::size>& data = vborect.data();
      rw::data::VBORect                                 originalVBORect = vborect;
      const std::array<float, rw::data::VBORect::size>& originalData = originalVBORect.data();
      float                                             height, width;

      using VtxIdx = rw::data::VBORect::VtxIdx;
      using TexIdx = rw::data::VBORect::TexIdx;

      // Test setH method.
      vborect.setH(rect.h);
      ASSERT_NEAR(vborect.h(), rect.h, rounding_error);
      height = data[static_cast<size_t>(VtxIdx::bl_y)] - data[static_cast<size_t>(VtxIdx::tl_y)];
      ASSERT_NEAR(height, rect.h, rounding_error);
      height = data[static_cast<size_t>(VtxIdx::br_y)] - data[static_cast<size_t>(VtxIdx::tr_y)];
      ASSERT_NEAR(height, rect.h, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_y)], data[static_cast<size_t>(VtxIdx::tr_y)], rounding_error);

      testEqToOriginal_VtxX(data, originalData);
      testEqToOriginal_VtxZ(data, originalData);
      testEqToOriginal_TexAll(data, originalData);

      originalVBORect = vborect;

      // Test setRect method.
      vborect.setRect(rect);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_x)], rect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_y)], rect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tr_x)], rect.x + rect.w, rounding_error, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tr_y)], rect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::br_x)], rect.x + rect.w, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::br_y)], rect.y + rect.h, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::bl_x)], rect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::bl_y)], rect.y + rect.h, rounding_error);

      testEqToOriginal_VtxZ(data, originalData);
      testEqToOriginal_TexAll(data, originalData);

      originalVBORect = vborect;

      // Test setTexH method.
      vborect.setTexH(texRect.h);
      ASSERT_NEAR(vborect.texH(), texRect.h, rounding_error);
      height = data[static_cast<size_t>(TexIdx::bl_y)] - data[static_cast<size_t>(TexIdx::tl_y)];
      ASSERT_NEAR(height, texRect.h, rounding_error);
      height = data[static_cast<size_t>(TexIdx::br_y)] - data[static_cast<size_t>(TexIdx::tr_y)];
      ASSERT_NEAR(height, texRect.h, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_y)], data[static_cast<size_t>(TexIdx::tr_y)], rounding_error);

      testEqToOriginal_VtxAll(data, originalData);
      testEqToOriginal_TexX(data, originalData);

      originalVBORect = vborect;

      // Test setTexRect method.
      vborect.setTexRect(texRect);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_x)], texRect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_y)], texRect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tr_x)], texRect.x + texRect.w, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tr_y)], texRect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::br_x)], texRect.x + texRect.w, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::br_y)], texRect.y + texRect.h, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::bl_x)], texRect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::bl_y)], texRect.y + texRect.h, rounding_error);

      testEqToOriginal_VtxAll(data, originalData);

      originalVBORect = vborect;

      // Test setTexW method.
      vborect.setTexW(texRect.w);
      ASSERT_NEAR(vborect.texW(), texRect.w, rounding_error);
      width = data[static_cast<size_t>(TexIdx::tr_x)] - data[static_cast<size_t>(TexIdx::tl_x)];
      ASSERT_NEAR(width, texRect.w, rounding_error);
      width = data[static_cast<size_t>(TexIdx::br_x)] - data[static_cast<size_t>(TexIdx::bl_x)];
      ASSERT_NEAR(width, texRect.w, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_x)], data[static_cast<size_t>(TexIdx::bl_x)], rounding_error);

      testEqToOriginal_VtxAll(data, originalData);
      testEqToOriginal_TexY(data, originalData);

      originalVBORect = vborect;

      // Test setTexX method.
      width = data[static_cast<size_t>(TexIdx::tr_x)] - data[static_cast<size_t>(TexIdx::tl_x)];
      vborect.setTexX(texRect.x);
      ASSERT_NEAR(vborect.texX(), texRect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_x)], texRect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::bl_x)], texRect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tr_x)], texRect.x + width, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::br_x)], texRect.x + width, rounding_error);

      testEqToOriginal_VtxAll(data, originalData);
      testEqToOriginal_TexY(data, originalData);

      originalVBORect = vborect;

      // Test setTexY method.
      height = data[static_cast<size_t>(TexIdx::bl_y)] - data[static_cast<size_t>(TexIdx::tl_y)];
      vborect.setTexY(texRect.y);
      ASSERT_NEAR(vborect.texY(), texRect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tl_y)], texRect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::tr_y)], texRect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::bl_y)], texRect.y + height, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(TexIdx::br_y)], texRect.y + height, rounding_error);

      testEqToOriginal_VtxAll(data, originalData);
      testEqToOriginal_TexX(data, originalData);

      originalVBORect = vborect;

      // Test setW method.
      vborect.setW(rect.w);
      ASSERT_NEAR(vborect.w(), rect.w, rounding_error);
      width = data[static_cast<size_t>(VtxIdx::tr_x)] - data[static_cast<size_t>(VtxIdx::tl_x)];
      ASSERT_NEAR(width, rect.w, rounding_error);
      width = data[static_cast<size_t>(VtxIdx::br_x)] - data[static_cast<size_t>(VtxIdx::bl_x)];
      ASSERT_NEAR(width, rect.w, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_x)], data[static_cast<size_t>(VtxIdx::bl_x)], rounding_error);

      testEqToOriginal_VtxY(data, originalData);
      testEqToOriginal_VtxZ(data, originalData);
      testEqToOriginal_TexAll(data, originalData);

      originalVBORect = vborect;

      // Test setX method.
      width = data[static_cast<size_t>(VtxIdx::tr_x)] - data[static_cast<size_t>(VtxIdx::tl_x)];
      vborect.setX(rect.x);
      ASSERT_NEAR(vborect.x(), rect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_x)], rect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::bl_x)], rect.x, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tr_x)], rect.x + width, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::br_x)], rect.x + width, rounding_error);

      testEqToOriginal_VtxY(data, originalData);
      testEqToOriginal_VtxZ(data, originalData);
      testEqToOriginal_TexAll(data, originalData);

      originalVBORect = vborect;

      // Test setY method.
      height = data[static_cast<size_t>(VtxIdx::bl_y)] - data[static_cast<size_t>(VtxIdx::tl_y)];
      vborect.setY(rect.y);
      ASSERT_NEAR(vborect.y(), rect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_y)], rect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tr_y)], rect.y, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::bl_y)], rect.y + height, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::br_y)], rect.y + height, rounding_error);

      testEqToOriginal_VtxX(data, originalData);
      testEqToOriginal_VtxZ(data, originalData);
      testEqToOriginal_TexAll(data, originalData);

      originalVBORect = vborect;

      // Test setZ method.
      vborect.setZ(z);
      ASSERT_NEAR(vborect.z(), z, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tl_z)], z, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::tr_z)], z, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::br_z)], z, rounding_error);
      ASSERT_NEAR(data[static_cast<size_t>(VtxIdx::bl_z)], z, rounding_error);

      testEqToOriginal_VtxX(data, originalData);
      testEqToOriginal_VtxY(data, originalData);
      testEqToOriginal_TexAll(data, originalData);
   }
} // namespace data_vborect_test

TEST(VBORectTest, Default)
{
   using namespace data_vborect_test;

   rw::data::VBORect                                 vborect;
   const std::array<float, rw::data::VBORect::size>& data = vborect.data();

   for (int i = 0; i < data.size(); ++i)
   {
      ASSERT_NEAR(data[i], defaultData[i], rounding_error);
   }

   testRect(vborect, rect1, texRect4);
   testRect(vborect, rect2, texRect3);
   testRect(vborect, rect3, texRect2);
   testRect(vborect, rect4, texRect1);
}

TEST(VBORectTest, CoordinateConstructor)
{
   using namespace data_vborect_test;

   rw::data::VBORect                                 vborect{ rect1 };
   const std::array<float, rw::data::VBORect::size>& data = vborect.data();

   for (int i = 0; i < data.size(); ++i)
   {
      ASSERT_NEAR(data[i], data_1_0[i], rounding_error);
   }

   testRect(vborect, rect1, texRect4);
   testRect(vborect, rect2, texRect3);
   testRect(vborect, rect3, texRect2);
   testRect(vborect, rect4, texRect1);
}

TEST(VBORectTest, FullConstructor)
{
   using namespace data_vborect_test;

   rw::data::VBORect                                 vborect1{ rect1, texRect4 };
   const std::array<float, rw::data::VBORect::size>& data1 = vborect1.data();

   for (int i = 0; i < data1.size(); ++i)
   {
      ASSERT_NEAR(data1[i], data_1_4[i], rounding_error);
   }

   testRect(vborect1, rect1, texRect4);
   testRect(vborect1, rect2, texRect3);
   testRect(vborect1, rect3, texRect2);
   testRect(vborect1, rect4, texRect1);

   rw::data::VBORect                                 vborect2{ rect2, texRect3 };
   const std::array<float, rw::data::VBORect::size>& data2 = vborect2.data();

   for (int i = 0; i < data2.size(); ++i)
   {
      ASSERT_NEAR(data2[i], data_2_3[i], rounding_error);
   }

   testRect(vborect2, rect1, texRect4);
   testRect(vborect2, rect2, texRect3);
   testRect(vborect2, rect3, texRect2);
   testRect(vborect2, rect4, texRect1);

   rw::data::VBORect                                 vborect3{ rect3, texRect2 };
   const std::array<float, rw::data::VBORect::size>& data3 = vborect3.data();

   for (int i = 0; i < data3.size(); ++i)
   {
      ASSERT_NEAR(data3[i], data_3_2[i], rounding_error);
   }

   testRect(vborect3, rect1, texRect4);
   testRect(vborect3, rect2, texRect3);
   testRect(vborect3, rect3, texRect2);
   testRect(vborect3, rect4, texRect1);

   rw::data::VBORect                                 vborect4{ rect4, texRect1 };
   const std::array<float, rw::data::VBORect::size>& data4 = vborect4.data();

   for (int i = 0; i < data4.size(); ++i)
   {
      ASSERT_NEAR(data4[i], data_4_1[i], rounding_error);
   }

   testRect(vborect4, rect1, texRect4);
   testRect(vborect4, rect2, texRect3);
   testRect(vborect4, rect3, texRect2);
   testRect(vborect4, rect4, texRect1);
}