/* Tests for rw::gl::GLBuffer */
#include "utility.hpp"

#include <RedWolf/gl/GLBuffer.hpp>
#include <SDL2.0.9/SDL.h>

#include <array>
#include <string_view>

enum class Error
{
    window_creation = 1,
    opengl
};

constexpr std::string_view win_name{ "GLBufferTest" }; // Name of the test window.
constexpr unsigned int def_uc{ 3 }; // Value for unsigned char buffers.
constexpr float def_f{ 3.3F }; // Value for float buffers. 
constexpr std::array<size_t, 3> sizes{ 1, 3, 64 }; // Buffer sizes to test.

int test();

template <typename T, size_t S>
void testBuffer(rw::gl::GLBuffer<T, S>& buf, std::array<T, S>& data);

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);
    
    int retVal{ test() };
    
    SDL_Quit();
    return retVal;
}

int test()
{
    rwt::window_t win{ rwt::setUpGLContext(win_name) };
    if (win == nullptr) return static_cast<int>(Error::window_creation);

    /* Test data. */
    std::array<unsigned int, sizes[0]> a_uc1{ def_uc };
    std::array<unsigned int, sizes[1]> a_uc3{ def_uc };
    std::array<unsigned int, sizes[2]> a_uc64{ def_uc };

    std::array<float, sizes[0]> a_f1{ def_f };
    std::array<float, sizes[1]> a_f3{ def_f };
    std::array<float, sizes[2]> a_f64{ def_f };

    /* Default buffers. */
    {
        /* Unsigned int. */
        rw::gl::GLBuffer<unsigned int, sizes[0]> uc1;
        testBuffer(uc1, a_uc1);
        rw::gl::GLBuffer<unsigned int, sizes[1]> uc3;
        testBuffer(uc3, a_uc3);
        rw::gl::GLBuffer<unsigned int, sizes[2]> uc64;
        testBuffer(uc64, a_uc64);

        /* Float. */
        rw::gl::GLBuffer<float, sizes[0]> f1;
        testBuffer(f1, a_f1);
        rw::gl::GLBuffer<float, sizes[1]> f3;
        testBuffer(f3, a_f3);
        rw::gl::GLBuffer<float, sizes[2]> f64;
        testBuffer(f64, a_f64);
    }

    /* Buffers with custom target. */
    {
        /* Unsigned int. */
        rw::gl::GLBuffer<unsigned int, sizes[0]> uc1{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(uc1, a_uc1);
        rw::gl::GLBuffer<unsigned int, sizes[1]> uc3{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(uc3, a_uc3);
        rw::gl::GLBuffer<unsigned int, sizes[2]> uc64{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(uc64, a_uc64);

        /* Float. */
        rw::gl::GLBuffer<float, sizes[0]> f1{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(f1, a_f1);
        rw::gl::GLBuffer<float, sizes[1]> f3{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(f3, a_f3);
        rw::gl::GLBuffer<float, sizes[2]> f64{ GL_ELEMENT_ARRAY_BUFFER };
        testBuffer(f64, a_f64);
    }

    /* Buffers with custom target and usage. */
    {
        /* Unsigned int. */
        rw::gl::GLBuffer<unsigned int, sizes[0]> uc1{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc1, a_uc1);
        rw::gl::GLBuffer<unsigned int, sizes[1]> uc3{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc3, a_uc3);
        rw::gl::GLBuffer<unsigned int, sizes[2]> uc64{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc64, a_uc64);

        /* Float. */
        rw::gl::GLBuffer<float, sizes[0]> f1{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f1, a_f1);
        rw::gl::GLBuffer<float, sizes[1]> f3{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f3, a_f3);
        rw::gl::GLBuffer<float, sizes[2]> f64{ GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f64, a_f64);
    }

    /* Completely custom buffers. */
    {
        /* Unsigned int. */
        rw::gl::GLBuffer<unsigned int, sizes[0]> uc1{ a_uc1, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc1, a_uc1);
        rw::gl::GLBuffer<unsigned int, sizes[1]> uc3{ a_uc3, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc3, a_uc3);
        rw::gl::GLBuffer<unsigned int, sizes[2]> uc64{ a_uc64, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(uc64, a_uc64);

        /* Float. */
        rw::gl::GLBuffer<float, sizes[0]> f1{ a_f1, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f1, a_f1);
        rw::gl::GLBuffer<float, sizes[1]> f3{ a_f3, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f3, a_f3);
        rw::gl::GLBuffer<float, sizes[2]> f64{ a_f64, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW };
        testBuffer(f64, a_f64);
    }

    /* Move buffers. */
    {
        rw::gl::GLBuffer<float, sizes[2]> buf1;
        testBuffer(buf1, a_f64);
        auto buf2{ std::move(buf1) };
        testBuffer(buf2, a_f64);
        auto buf3 = std::move(buf2);
        testBuffer(buf3, a_f64);

        rw::gl::GLBuffer<float, sizes[2]> buf4{ a_f64, GL_ARRAY_BUFFER, GL_STATIC_DRAW };
        buf4 = std::move(buf3);
        testBuffer(buf4, a_f64);
    }


    if (rwt::g_glError) return static_cast<int>(Error::opengl);
    return 0;
}

template<typename T, size_t S>
void testBuffer(rw::gl::GLBuffer<T, S>& buf, std::array<T, S>& data)
{
    buf.bind();
    buf.setTarget(GL_ELEMENT_ARRAY_BUFFER);
    buf.bind();
    buf.setTarget(GL_ARRAY_BUFFER);
    buf.bind();
    buf.setData(data);
    buf.bind();
}
