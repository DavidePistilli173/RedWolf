#pragma once

#include "redwolf/common.hpp"
#include "render_packet.hpp"

#include <string_view>

namespace rw {
    /**
     * @brief Renderer frontend, API-agnostic.
     */
    class Renderer {
     public:
        /**
         * @brief Draw a frame.
         * @param packet Data required to draw the required frame.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool draw_frame(const RenderPacket& packet);

        /**
         * @brief Initialise the renderer backend.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Called on viewport resize.
         * @param width New width.
         * @param height New height.
         */
        static void on_resize(u16 width, u16 height);

        /**
         * @brief Shutdown the renderer.
         */
        static void shutdown();

     private:
        Renderer() = default;
    };
} // namespace rw