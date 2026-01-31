module;

#include "RedWolf/macros.hpp"

#include <array>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <span>
#include <string_view>
#include <thread>
#include <vector>

export module redwolf.gfx.font;

import redwolf.gfx.common;
import redwolf.gfx.texture_2d;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class representing a font resource.
     */
    class Font {
     public:
        /**
         * @brief Default character ranges to load from the font.
         */
        static constexpr std::array<std::pair<uint32_t, uint32_t>, 3U> default_char_ranges = {
            std::pair{ 0x0020U, 0x00FFU } // Basic Latin + Latin-1 Supplement
        };

        static constexpr double   font_scale{ 1.0 };                        /**< Scale factor for font loading. */
        static constexpr double   pixel_range{ 2.0 };                       /**< Pixel range for atlas packing. */
        static constexpr double   packer_scale{ 80.0 };                     /**< Scale for the atlas packer. */
        static constexpr uint64_t lcg_multiplier{ 6364136223846793005ULL }; /**< LCG multiplier taken from msdf-atlas-gen. */
        static constexpr uint64_t lcg_increment{ 1442695040888963407ULL };  /**< LCG increment taken from msdf-atlas-gen. */
        static constexpr double   default_angle_threshold{ 3.0 };           /**< Default angle threshold for edge coloring. */

        /**
         * @brief Constructor.
         * @param path Path to the font file.
         */
        explicit Font(const std::string_view path) {
            // Initialise FreeType.
            msdfgen::FreetypeHandle* ft{ msdfgen::initializeFreetype() };
            if (nullptr == ft) {
                RW_CORE_ERR("Failed to initialize FreeType library for font loading.");
                return;
            }

            // Load font.
            msdfgen::FontHandle* font{ msdfgen::loadFont(ft, std::string{ path }.c_str()) };
            if (nullptr == font) {
                msdfgen::deinitializeFreetype(ft);
                RW_CORE_ERR("Failed to load font from path: {}", path);
                return;
            }

            // Create character set.
            msdf_atlas::Charset charset;
            for (const auto [first, last] : default_char_ranges) {
                for (uint32_t c = first; c <= last; ++c) {
                    charset.add(c);
                }
            }

            // Load glyphs from font.
            font_geometry_ = msdf_atlas::FontGeometry(&glyphs_);
            const int32_t loaded_glyphs{ font_geometry_.loadCharset(font, font_scale, charset) };
            RW_CORE_TRACE("Loaded {}/{} glyphs from font at path: {}", loaded_glyphs, charset.size(), path);

            // Apply MSDF edge coloring.
            for (msdf_atlas::GlyphGeometry& glyph : glyphs_) {
                glyph.edgeColoring(msdfgen::edgeColoringInkTrap, default_angle_threshold, 0);
            }

            // Configure atlas packer and pack.
            msdf_atlas::TightAtlasPacker atlas_packer;
            atlas_packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::POWER_OF_TWO_SQUARE);
            atlas_packer.setMinimumScale(packer_scale);
            atlas_packer.setPixelRange(pixel_range);
            atlas_packer.setMiterLimit(1.0);
            if (const int32_t remaining{ atlas_packer.pack(glyphs_.data(), static_cast<int32_t>(glyphs_.size())) }; 0 != remaining) {
                RW_CORE_ERR("Failed to pack {} glyphs into the atlas for font at path: {}", remaining, path);
                return;
            }

            int32_t width{ 0 };
            int32_t height{ 0 };
            atlas_packer.getDimensions(width, height);
            const double scale{ atlas_packer.getScale() };

            // Create atlas texture.
            auto texture_res{ create_and_cache_atlas_<uint8_t, float, 3, msdf_atlas::msdfGenerator>(
                path, scale, glyphs_, font_geometry_, width, height) };
            if (!texture_res.has_value()) {
                RW_CORE_ERR("Failed to create texture atlas for font at path: {}", path);
                return;
            }
            atlas_texture_ = std::move(texture_res.value());

            // Clean up
            msdfgen::destroyFont(font);
            msdfgen::deinitializeFreetype(ft);
        }

        /**
         * @brief Get the atlas texture of the font.
         * @return Atlas teture of the font.
         */
        [[nodiscard]] const rw::gfx::Texture2D& atlas_texture() const noexcept {
            return atlas_texture_;
        }

        /**
         * @brief Get the font geometry.
         * @return Font geometry.
         */
        [[nodiscard]] const msdf_atlas::FontGeometry& font_geometry() const noexcept {
            return font_geometry_;
        }

     private:
        template<typename T, typename S, int32_t N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
        [[nodiscard]] std::optional<rw::gfx::Texture2D> create_and_cache_atlas_(
            const std::string_view                        font_name,
            double                                        font_size,
            const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
            const msdf_atlas::FontGeometry&               font_geometry,
            const uint32_t                                width,
            const uint32_t                                height) {
            msdf_atlas::GeneratorAttributes attributes;
            attributes.config.overlapSupport = true;
            attributes.scanlinePass          = true;

            msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
            generator.setAttributes(attributes);
            generator.setThreadCount(std::thread::hardware_concurrency());
            generator.generate(glyphs.data(), static_cast<int32_t>(glyphs.size()));
            msdfgen::BitmapConstRef<T, N> bitmap{ generator.atlasStorage() };
            if (0 > bitmap.width || 0 > bitmap.height) {
                RW_CORE_ERR("Invalid size {}x{} for font atlas: {}", bitmap.width, bitmap.height, font_name);
                return {};
            }

            rw::gfx::Texture2D texture;
            texture.set_data(
                std::span(bitmap.pixels, bitmap.width * bitmap.height * N),
                rw::gfx::TextureDescriptor{ .width  = static_cast<uint32_t>(bitmap.width),
                                            .height = static_cast<uint32_t>(bitmap.height),
                                            .format = rw::gfx::TextureFormat::rgb8 });
            return texture;
        }

        std::vector<msdf_atlas::GlyphGeometry> glyphs_;        /**< Vector of glyph geometries contained in the font. */
        rw::gfx::Texture2D                     atlas_texture_; /**< Texture atlas containing the rendered font glyphs. */
        msdf_atlas::FontGeometry               font_geometry_; /**< Font geometry containing glyph metrics and other data. */
    };
} // namespace rw::gfx
