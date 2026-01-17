//
// Created by david on 08/09/2025.
//

#ifndef SRC_REDWOLF_GL_TEXTURE_HPP
#define SRC_REDWOLF_GL_TEXTURE_HPP

#include "RedWolf/math/geometry.hpp"
#include "RedWolf/math/math.hpp"
#include "Redwolf/gfx/common.hpp"

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
         * @brief Constructor that creates an empty texture.
         */
        explicit Texture2D();

        /**
         * @brief Constructor that loads a texture from a file.
         * @param path Path to the texture file.
         */
        explicit Texture2D(const std::string_view path);

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
         * @brief Get the renderer Id of the texture.
         * @return Renderer ID of the texture.
         */
        [[nodiscard]] uint32_t renderer_id() const noexcept {
            return id_;
        }

        /**
         * @brief Set the texture data.
         * @param data Data to set into the texture.
         * @param descriptor Parameters for setting the data correctly.
         */
        void set_data(const std::span<const uint8_t> data, const TextureDescriptor& descriptor);

        /**
         * @brief Get the width of the texture.
         * @return Width of the texture in pixels.
         */
        [[nodiscard]] uint32_t width() const;

     private:
        /**
         * @brief Set the internal data format of the texture.
         * @param format User-specified texture format.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool set_data_format_(const TextureFormat format);

        uint32_t id_{ 0U }; /**< OpenGL texture ID. */

        std::string path_;                       /**< Path to the texture file. */
        uint32_t    width_{ 0U };                /**< Width of the texture. [px] */
        uint32_t    height_{ 0U };               /**< Height of the texture. [px] */
        GLenum      internal_format_{ GL_RGB8 }; /**< Internal texture format. */
        GLenum      data_format_{ GL_RGB };      /**< Texture data format. */
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_TEXTURE_HPP
