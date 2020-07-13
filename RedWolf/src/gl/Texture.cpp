#include "RedWolf/gl/Texture.hpp"

#include "RedWolf/Log.hpp"

#include <SDL2_image2.0.4/SDL_image.h>
#include <SDL2_ttf2.0.15/SDL_ttf.h>

namespace rw::gl
{
   Texture::Texture()
   {
      glGenTextures(1, &id_);
   }

   /**
        \param fileName Path to the file containing the texture to be loaded.
        Currently supports .jpg and .png files.
    */
   Texture::Texture(std::string_view fileName)
   {
      /* Load the image from fileName. */
      surf_ptr_t texSurf{ IMG_Load(fileName.data()), SDL_FreeSurface };
      if (texSurf == nullptr)
      {
         Log::core_err("Failed to load texture \"{0}\".", fileName.data());
         throw std::exception();
      }

      /* Create and load the OpenGL texture. */
      loadTex_(texSurf, DataFormat::png_jpg);
   }

   /**
        \param text Text to be rendered.
        \param textColour Colour of the text.
        \param font Font for the text.
    */
   Texture::Texture(std::string_view text, SDL_Color textColour, const font_ptr_t& font)
   {
      /* Render a surface from input text. */
      surf_ptr_t texSurf{ TTF_RenderUTF8_Blended(font.get(), text.data(), textColour), SDL_FreeSurface };
      if (texSurf == nullptr)
      {
         Log::core_err("Failed to create surface from text \"{0}\"", text.data());
         throw std::exception();
      }

      /* Create and load the OpenGL texture. */
      loadTex_(texSurf, DataFormat::blended_text);
   }

   Texture::~Texture()
   {
      glDeleteTextures(1, &id_);
   }

   Texture::Texture(Texture&& tex) noexcept :
      id_{ tex.id_ },
      wrapX_{ tex.wrapX_ },
      wrapY_{ tex.wrapY_ },
      minFilter_{ tex.minFilter_ },
      magFilter_{ tex.magFilter_ },
      size_{ tex.size_ }
   {
      tex.id_ = 0;
      tex.size_ = { 0, 0 };
   }

   Texture& Texture::operator=(Texture&& tex) noexcept
   {
      if (&tex != this)
      {
         glDeleteTextures(1, &id_);

         id_ = tex.id_;
         tex.id_ = 0;

         wrapX_ = tex.wrapX_;
         wrapY_ = tex.wrapY_;
         minFilter_ = tex.minFilter_;
         magFilter_ = tex.magFilter_;

         size_ = tex.size_;
         tex.size_ = { 0, 0 };
      }
      return *this;
   }

   void Texture::bind() const
   {
      glBindTexture(GL_TEXTURE_2D, id_);
   }

   /**
        \return Height of the texture in pixels.
    */
   int Texture::getH() const
   {
      return size_.h;
   }

   /**
        \return Width of the texture in pixels.
    */
   int Texture::getW() const
   {
      return size_.w;
   }

   /**
        \return Size of the texture in pixels.
    */
   Size<int> Texture::getSize() const
   {
      return size_;
   }

   /**
        \param val New downscaling setting.
    */
   void Texture::setMinFilt(GLenum val)
   {
      bind();
      minFilter_ = val;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, val);
   }

   /**
        \param val New upscaling setting.
    */
   void Texture::setMagFilt(GLenum val)
   {
      bind();
      magFilter_ = val;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, val);
   }

   /**
        \param val New setting for horizontal wrapping.
    */
   void Texture::setWrapX(GLenum val)
   {
      bind();
      wrapX_ = val;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, val);
   }

   /**
        \param val New setting for vertical wrapping.
    */
   void Texture::setWrapY(GLenum val)
   {
      bind();
      wrapY_ = val;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, val);
   }

   /**
        \param texSurf Surface containing texture data.
        \param format Data formatting of texSurf.
    */
   void Texture::loadTex_(surf_ptr_t& texSurf, DataFormat format)
   {
      /* Store the size of the texture. */
      size_ = { texSurf->w, texSurf->h };

      /* Generate a new OpenGL texture and bind it. */
      glGenTextures(1, &id_);
      bind();

      /* Set OpenGL texture settings. */
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_);

      /* Set image and pixel formats. */
      GLint  imgFormat{ GL_RGB };
      GLenum pxlFormat{ GL_UNSIGNED_BYTE };

      switch (format)
      {
      case DataFormat::png_jpg:
         if (texSurf->format->BitsPerPixel == 4) imgFormat = GL_RGBA;
         break;
      case DataFormat::blended_text:
         imgFormat = GL_RGBA;
         texSurf.reset(SDL_ConvertSurfaceFormat(texSurf.get(), SDL_PIXELFORMAT_ABGR8888, 0));
         break;
      }

      /* Load the texture into memory. */
      glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, size_.w, size_.h, 0, imgFormat, pxlFormat, texSurf->pixels);
      /* Generate mipmaps for the current texture. */
      glGenerateMipmap(GL_TEXTURE_2D);
   }
} // namespace rw::gl