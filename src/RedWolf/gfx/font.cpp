//
// Created by david on 10/01/2026.
//

#include "font.hpp"

#include "RedWolf/util/logger.hpp"

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdfgen/msdfgen.h>

rw::gfx::Font::Font(const std::string_view path) {
    msdfgen::FreetypeHandle* ft{ msdfgen::initializeFreetype() };
    if (nullptr == ft) {
        RW_CORE_ERR("Failed to initialize FreeType library for font loading.");
        return;
    }

    msdfgen::FontHandle* font{ msdfgen::loadFont(ft, std::string{ path }.c_str()) };
    if (nullptr == font) {
        msdfgen::deinitializeFreetype(ft);
        RW_CORE_ERR("Failed to load font from path: {}", path);
        return;
    }

    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, font, 'A')) {
        RW_CORE_ERR("Failed to load glyph for character 'A' from font at path: {}", path);
        return;
    }

    msdfgen::edgeColoringSimple(shape, 3.0);
    msdfgen::Bitmap<float, 3> msdf(32, 32);
    msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
    msdfgen::savePng(msdf, "output.png");

    // Clean up
    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);
}