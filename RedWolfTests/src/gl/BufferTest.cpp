/* Tests for rw::gl::Buffer */
#include <gtest/gtest.h>
#include <RedWolf/gl/Buffer.hpp>
#include <RedWolf/gl/error.hpp>
#include <RedWolf/Log.hpp>
#include <SDL2.0.9/SDL.h>

#include <array>

template <typename T, size_t S>
void testBuffer(rw::gl::Buffer<T, S>& buf, const std::array<T, S> data);

constexpr unsigned int def_uc{ 3 }; // Value for unsigned char buffers.
constexpr float def_f{ 3.3F }; // Value for float buffers. 
constexpr std::array<size_t, 3> sizes{ 1, 3, 64 }; // Buffer sizes to test.

/* Test data. */
constexpr std::array<unsigned int, sizes[0]> a_uc1{ def_uc };
constexpr std::array<unsigned int, sizes[1]> a_uc3{ def_uc };
constexpr std::array<unsigned int, sizes[2]> a_uc64{ def_uc };

constexpr std::array<float, sizes[0]> a_f1{ def_f };
constexpr std::array<float, sizes[1]> a_f3{ def_f };
constexpr std::array<float, sizes[2]> a_f64{ def_f };

TEST(GLBuffer, DefaultBuffers)
{
    /* Unsigned int. */
    rw::gl::Buffer<unsigned int, sizes[0]> uc1;
    testBuffer(uc1, a_uc1);
    rw::gl::Buffer<unsigned int, sizes[1]> uc3;
    testBuffer(uc3, a_uc3);
    rw::gl::Buffer<unsigned int, sizes[2]> uc64;
    testBuffer(uc64, a_uc64);

    /* Float. */
    rw::gl::Buffer<float, sizes[0]> f1;
    testBuffer(f1, a_f1);
    rw::gl::Buffer<float, sizes[1]> f3;
    testBuffer(f3, a_f3);
    rw::gl::Buffer<float, sizes[2]> f64;
    testBuffer(f64, a_f64);
}

TEST(GLBuffer, CustomTarget)
{
    /* Unsigned int. */
    rw::gl::Buffer<unsigned int, sizes[0]> uc1{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(uc1, a_uc1);
    rw::gl::Buffer<unsigned int, sizes[1]> uc3{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(uc3, a_uc3);
    rw::gl::Buffer<unsigned int, sizes[2]> uc64{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(uc64, a_uc64);

    /* Float. */
    rw::gl::Buffer<float, sizes[0]> f1{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(f1, a_f1);
    rw::gl::Buffer<float, sizes[1]> f3{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(f3, a_f3);
    rw::gl::Buffer<float, sizes[2]> f64{ GL_ELEMENT_ARRAY_BUFFER };
    testBuffer(f64, a_f64);
}

TEST(GLBuffer, CustomTargetAndUsage)
{
    /* Unsigned int. */
    rw::gl::Buffer<unsigned int, sizes[0]> uc1{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc1, a_uc1);
    rw::gl::Buffer<unsigned int, sizes[1]> uc3{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc3, a_uc3);
    rw::gl::Buffer<unsigned int, sizes[2]> uc64{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc64, a_uc64);

    /* Float. */
    rw::gl::Buffer<float, sizes[0]> f1{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f1, a_f1);
    rw::gl::Buffer<float, sizes[1]> f3{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f3, a_f3);
    rw::gl::Buffer<float, sizes[2]> f64{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f64, a_f64);
}

TEST(GLBuffer, FullCustom)
{
    /* Unsigned int. */
    rw::gl::Buffer<unsigned int, sizes[0]> uc1{ a_uc1, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc1, a_uc1);
    rw::gl::Buffer<unsigned int, sizes[1]> uc3{ a_uc3, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc3, a_uc3);
    rw::gl::Buffer<unsigned int, sizes[2]> uc64{ a_uc64, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(uc64, a_uc64);

    /* Float. */
    rw::gl::Buffer<float, sizes[0]> f1{ a_f1, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f1, a_f1);
    rw::gl::Buffer<float, sizes[1]> f3{ a_f3, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f3, a_f3);
    rw::gl::Buffer<float, sizes[2]> f64{ a_f64, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
    testBuffer(f64, a_f64);
}

TEST(GLBuffer, Move)
{
    rw::gl::Buffer<float, sizes[2]> buf1;
    testBuffer(buf1, a_f64);
    auto buf2{ std::move(buf1) };
    testBuffer(buf2, a_f64);
    auto buf3 = std::move(buf2);
    testBuffer(buf3, a_f64);

    rw::gl::Buffer<float, sizes[2]> buf4{ a_f64, GL_ARRAY_BUFFER, GL_STATIC_DRAW };
    buf4 = std::move(buf3);
    testBuffer(buf4, a_f64);
}

template<typename T, size_t S>
void testBuffer(rw::gl::Buffer<T, S>& buf, const std::array<T, S> data)
{
    buf.bind();
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.setTarget(GL_ELEMENT_ARRAY_BUFFER);
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.bind();
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.setTarget(GL_ARRAY_BUFFER);
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.bind();
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.setData(data);
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
    buf.bind();
    ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
}
