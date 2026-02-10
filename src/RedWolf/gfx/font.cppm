module;

#include <array>
#include <memory>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <string_view>
#include <thread>
#include <vector>

export module redwolf.gfx.font;

import redwolf.common;
import redwolf.gfx.common;
import redwolf.gfx.texture2d;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class representing a font resource.
     */
    struct Font {
        /**
         * @brief Default character ranges to load from the font.
         */
        static constexpr std::array<std::pair<uint32_t, uint32_t>, 3U> default_char_ranges = {
            std::pair{ 0x0020U, 0x00FFU } // Basic Latin + Latin-1 Supplement
        };

        std::unique_ptr<std::vector<msdf_atlas::GlyphGeometry>> glyphs;        /**< Vector of glyph geometries contained in the font. */
        msdf_atlas::FontGeometry                                font_geometry; /**< Metrics for all the glyphs in the font. */
        Handle<Texture2D>                                       atlas_texture; /**< Texture atlas containing the rendered font glyphs. */
    };
} // namespace rw::gfx
