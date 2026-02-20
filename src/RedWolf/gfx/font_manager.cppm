module;

#include <cstdint>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <span>
#include <string_view>
#include <thread>
#include <utility>

export module redwolf.gfx.font_manager;

import redwolf.core.handle;
import redwolf.core.asset_library;
import redwolf.gfx.common;
import redwolf.gfx.font;
import redwolf.gfx.texture2d;
import redwolf.gfx.texture2d_manager;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class for managing fonts.
     */
    class FontManager {
     public:
        static constexpr double   font_scale{ 1.0 };                        /**< Scale factor for font loading. */
        static constexpr double   pixel_range{ 2.0 };                       /**< Pixel range for atlas packing. */
        static constexpr double   packer_scale{ 80.0 };                     /**< Scale for the atlas packer. */
        static constexpr uint64_t lcg_multiplier{ 6364136223846793005ULL }; /**< LCG multiplier taken from msdf-atlas-gen. */
        static constexpr uint64_t lcg_increment{ 1442695040888963407ULL };  /**< LCG increment taken from msdf-atlas-gen. */
        static constexpr double   default_angle_threshold{ 3.0 };           /**< Default angle threshold for edge coloring. */

        /**
         * @brief Constructor.
         */
        FontManager() = default;

        /**
         * @brief Bind the texture atlas of a given font.
         * @param font_handle Handle to the font to bind the atlas of.
         * @param texture_manager Texture manager that holds the texture atlas.
         */
        void bind_atlas(const Handle<Font> font_handle, const Texture2DManager& texture_manager) {
            fonts_.visit<void>(font_handle, [&texture_manager](const Font& font) { texture_manager.bind(font.atlas_texture, 0); });
        }

        /**
         * @brief Load a new font from a file path.
         * @param path Path to load the font from.
         * @param texture_manager Texture manager for storing the font atlas.
         * @return Handle to the newly created path.
         */
        [[nodiscard]] Handle<Font> new_from_path(const std::string_view path, Texture2DManager& texture_manager) {
            // Initialise FreeType.
            msdfgen::FreetypeHandle* ft{ msdfgen::initializeFreetype() };
            if (nullptr == ft) {
                rw::err("Failed to initialize FreeType library for font loading.");
                return {};
            }

            // Load font.
            msdfgen::FontHandle* font_raw{ msdfgen::loadFont(ft, std::string{ path }.c_str()) };
            if (nullptr == font_raw) {
                msdfgen::deinitializeFreetype(ft);
                rw::err("Failed to load font from path: {}", path);
                return {};
            }

            // Create character set.
            msdf_atlas::Charset charset;
            for (const auto [first, last] : Font::default_char_ranges) {
                for (uint32_t c = first; c <= last; ++c) {
                    charset.add(c);
                }
            }

            auto [font, font_handle]{ fonts_.create() };

            // Load glyphs from font.
            font.glyphs        = std::make_unique<std::vector<msdf_atlas::GlyphGeometry>>();
            font.font_geometry = msdf_atlas::FontGeometry(font.glyphs.get());
            const int32_t loaded_glyphs{ font.font_geometry.loadCharset(font_raw, font_scale, charset) };
            rw::trace("Loaded {}/{} glyphs from font at path: {}", loaded_glyphs, charset.size(), path);

            // Apply MSDF edge coloring.
            for (msdf_atlas::GlyphGeometry& glyph : *font.glyphs) {
                glyph.edgeColoring(msdfgen::edgeColoringInkTrap, default_angle_threshold, 0);
            }

            // Configure atlas packer and pack.
            msdf_atlas::TightAtlasPacker atlas_packer;
            atlas_packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::POWER_OF_TWO_SQUARE);
            atlas_packer.setMinimumScale(packer_scale);
            atlas_packer.setPixelRange(pixel_range);
            atlas_packer.setMiterLimit(1.0);
            if (const int32_t remaining{ atlas_packer.pack(font.glyphs->data(), static_cast<int32_t>(font.glyphs->size())) };
                0 != remaining) {
                rw::err("Failed to pack {} glyphs into the atlas for font at path: {}", remaining, path);
                fonts_.remove(font_handle, []([[maybe_unused]] const Font& font) {});
                return {};
            }

            int32_t width{ 0 };
            int32_t height{ 0 };
            atlas_packer.getDimensions(width, height);

            // Create atlas texture.
            font.atlas_texture =
                create_and_cache_atlas_<uint8_t, float, 3, msdf_atlas::msdfGenerator>(path, *font.glyphs, width, height, texture_manager);
            if (font.atlas_texture.invalid()) {
                rw::err("Failed to create texture atlas for font at path: {}", path);
                fonts_.remove(font_handle, []([[maybe_unused]] const Font& font) {});
                return {};
            }

            // Clean up
            msdfgen::destroyFont(font_raw);
            msdfgen::deinitializeFreetype(ft);

            return font_handle;
        }

        /**
         * @brief Get a raw reference to a given font.
         * @details Be careful with the lifetime of the retrieved reference.
         * @param font_handle Handle of the font to get.
         * @return Reference to the given font, if it exists.
         */
        [[nodiscard]] std::optional<std::reference_wrapper<Font>> unsafe_get_raw(const Handle<Font> font_handle) {
            return fonts_.unsafe_get_raw(font_handle);
        }

     private:
        template<typename T, typename S, int32_t N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
        [[nodiscard]] Handle<Texture2D> create_and_cache_atlas_(
            const std::string_view                        font_name,
            const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
            const uint32_t                                width,
            const uint32_t                                height,
            Texture2DManager&                             texture_manager) {
            msdf_atlas::GeneratorAttributes attributes;
            attributes.config.overlapSupport = true;
            attributes.scanlinePass          = true;

            msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
            generator.setAttributes(attributes);
            generator.setThreadCount(std::thread::hardware_concurrency());
            generator.generate(glyphs.data(), static_cast<int32_t>(glyphs.size()));
            msdfgen::BitmapConstRef<T, N> bitmap{ generator.atlasStorage() };
            if (0 > bitmap.width || 0 > bitmap.height) {
                rw::err("Invalid size {}x{} for font atlas: {}", bitmap.width, bitmap.height, font_name);
                return {};
            }

            Handle<Texture2D> texture{ texture_manager.new_empty() };
            texture_manager.set_data(
                texture,
                std::span(bitmap.pixels, bitmap.width * bitmap.height * N),
                rw::gfx::TextureDescriptor{ .width  = static_cast<uint32_t>(bitmap.width),
                                            .height = static_cast<uint32_t>(bitmap.height),
                                            .format = rw::gfx::TextureFormat::rgb8 });
            return texture;
        }

        rw::core::AssetLibrary<Font> fonts_; /**< Actual storage for fonts. */
    };
} // namespace rw::gfx
