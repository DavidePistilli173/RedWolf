//
// Created by david on 10/01/2026.
//

#ifndef SRC_REDWOLF_FONT_HPP
#define SRC_REDWOLF_FONT_HPP

#include <string_view>

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
        Font(const std::string_view path);
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_FONT_HPP
