//
// Created by david on 08/09/2025.
//

#ifndef SRC_REDWOLF_GL_TEXTURE_HPP
#define SRC_REDWOLF_GL_TEXTURE_HPP

#include "RedWolf/math/geometry.hpp"
#include "RedWolf/math/math.hpp"

#include <array>
#include <cstdint>
#include <glad/glad.h>
#include <span>
#include <string>
#include <string_view>

namespace rw::gfx::api::gl {
    /**
     * @brief 2D texture class for OpenGL.
     */
    class Texture2D {
     public:
        using SubRegion = std::array<rw::math::Vec2, 4>; /**< Texture coordinates of a texture sub-region. */
        /**
         * @brief Constructor that loads a texture from a file.
         * @param path Path to the texture file.
         */
        explicit Texture2D(const std::string_view path);

        /**
         * @brief Constructor that creates a texture with a given size.
         * @param width Width of the texture.
         * @param height Height of the texture.
         */
        explicit Texture2D(const uint32_t width, const uint32_t height);

        /**
         * @brief Destructor.
         */
        ~Texture2D();

        /**
         * @brief Copy constructor.
         */
        Texture2D(const Texture2D&) = delete;

        /**
         * @brief Move constructor.
         */
        Texture2D(Texture2D&& other) noexcept;

        /**
         * @brief Copy-assignment operator.
         */
        Texture2D& operator=(const Texture2D&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        Texture2D& operator=(Texture2D&& other) noexcept;

        /**
         * @brief Bind the texture to the current context.
         * @param slot Texture slot to bind to.
         */
        void bind(const uint32_t slot) const;

        /**
         * @brief Get the number of bytes required for each pixel.
         * @return Bytes required for each pixel.
         */
        [[nodiscard]] uint32_t bytes_per_pixel() const;

        /**
         * @brief Compute a sub-region of the texture.
         * @param region Region of the texture to compute, in pixels.
         * @return Texture coordinates of the sub-region.
         */
        [[nodiscard]] SubRegion compute_sub_region(const rw::math::Rect<float>& region) const;

        /**
         * @brief Get the height of the texture.
         * @return Height of the texture in pixels.
         */
        [[nodiscard]] uint32_t height() const;

        /**
         * @brief Get the path to the texture source file.
         * @return Path to the texture file.
         */
        [[nodiscard]] const std::string& path() const;

        /**
         * @brief Set the texture data.
         * @param data Data to set into the texture.
         */
        void set_data(const std::span<const uint8_t> data);

        /**
         * @brief Get the width of the texture.
         * @return Width of the texture in pixels.
         */
        [[nodiscard]] uint32_t width() const;

     private:
        uint32_t id_{ 0U }; /**< OpenGL texture ID. */

        std::string path_;                       /**< Path to the texture file. */
        uint32_t    width_{ 0U };                /**< Width of the texture. [px] */
        uint32_t    height_{ 0U };               /**< Height of the texture. [px] */
        GLenum      internal_format_{ GL_RGB8 }; /**< Internal texture format. */
        GLenum      data_format_{ GL_RGB };      /**< Texture data format. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_TEXTURE_HPP
