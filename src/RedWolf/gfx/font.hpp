//
// Created by david on 10/01/2026.
//

#ifndef SRC_REDWOLF_FONT_HPP
#define SRC_REDWOLF_FONT_HPP

#include "RedWolf/gfx/gfx.hpp"

#include <msdf-atlas-gen/GlyphGeometry.h>
#include <string_view>
#include <vector>

namespace rw::gfx {
    /**
     * @brief Class representing a font resource.
     */
    class Font {
     public:
        /**
         * @brief Constructor.
         * @param path Path to the font file.
         */
        explicit Font(const std::string_view path);

        /**
         * @brief Get the atlas texture of the font.
         * @return Atlas teture of the font.
         */
        [[nodiscard]] const rw::gfx::Texture2D& atlas_texture() const noexcept {
            return atlas_texture_;
        }

     private:
        std::vector<msdf_atlas::GlyphGeometry> glyphs_;        /**< Vector of glyph geometries contained in the font. */
        rw::gfx::Texture2D                     atlas_texture_; /**< Texture atlas containing the rendered font glyphs. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_FONT_HPP
