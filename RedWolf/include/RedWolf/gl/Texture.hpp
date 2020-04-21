#ifndef RW_GL_TEXTURE_HPP
#define RW_GL_TEXTURE_HPP

#include "RedWolf/core.hpp"

#include "RedWolf/utility.hpp"

#include <glad/glad.h>
#include <SDL2.0.9/SDL.h>

#include <string_view>

namespace rw::gl
{
    /** \brief Handler for an OpenGL texture. */
    class RW_API Texture
    {
    public:
        /** \brief Possible image formats to load. */
        enum class DataFormat
        {
            png_jpg, /**< Standard PNG or JPG images. */
            blended_text /**< Image rendered from text with alpha blending. */
        };

        /** \brief Create an empty texture. */
        Texture();
        /** \brief Create a texture from a given file. */
        explicit Texture(std::string_view fileName);
        /** \brief Create a texture representing the given text. */
        Texture(std::string_view text, SDL_Color textColour, const font_ptr_t& font);

        /** \brief Destructor. */
        ~Texture();

        /** \brief Textures cannot be copied. */
        Texture(const Texture&) = delete;
        /** \brief Move constructor. */
        Texture(Texture&& tex) noexcept;

        /** \brief Texture cannot be copied. */
        Texture& operator=(const Texture&) = delete;
        /** \brief Move assignment operator. */
        Texture& operator=(Texture&& tex) noexcept;

        /** \brief Bind the current texture for rendering. */
        void bind() const;

        /** \brief Get the texture's height in pixels. */
        [[nodiscard]] int getH() const;
        /** \brief Get the texture's width in pixels. */
        [[nodiscard]] int getW() const;
        /** \brief Get the texture's size in pixels. */
        [[nodiscard]] Size<int> getSize() const;

        /** \brief Set the downscaling interpolation behaviour. */
        void setMinFilt(GLenum val);
        /** \brief Set the upscaling interpolation behaviour. */
        void setMagFilt(GLenum val);
        /** \brief Set the wrapping behaviour on the X axis. */
        void setWrapX(GLenum val);
        /** \brief Set the wrapping behaviour on the Y axis. */
        void setWrapY(GLenum val);

    private:
        /**< \brief Load a surface into OpenGL as a texture. */
        void loadTex_(surf_ptr_t& texSurf, DataFormat format);

        GLuint id_{ 0 }; /**< Texture id. */
        GLenum wrapX_{ GL_REPEAT }; /**< Current wrapping behaviour on the X axis. */
        GLenum wrapY_{ GL_REPEAT }; /**< Current wrapping behaviour on the Y axis. */
        GLenum minFilter_{ GL_NEAREST_MIPMAP_LINEAR }; /**< Current settings for downscaling. */
        GLenum magFilter_{ GL_LINEAR }; /**< Current settings for upscaling. */
        Size<int> size_{ 0,0 }; /**< Size of the texture in pixels. */
    };
}

#endif