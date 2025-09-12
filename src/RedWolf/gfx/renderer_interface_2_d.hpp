//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP
#define SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP

#include "gfx.hpp"
#include "renderer_2_d.hpp"

namespace rw::gfx {
    /**
     * @brief Interface for the strictly 2D renderer.
     */
    class RendererInterface2D {
     public:
        /**
         * @brief Constructor.
         * @param renderer Renderer instance to connect to.
         */
        explicit RendererInterface2D(std::shared_ptr<Renderer2D> renderer);

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera);

        /**
         * @brief Draw a quad to the screen.
         * @param shader_id ID of the shader to use for rendering.
         * @param position Position of the quad.
         * @param size Size of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(const uint64_t shader_id, const rw::math::Vec2& position, const rw::math::Vec2& size, const rw::math::Vec4& color);

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order..
         * @param shader_id ID of the shader to use for rendering.
         * @param position Position of the quad.
         * @param size Size of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(const uint64_t shader_id, const rw::math::Vec3& position, const rw::math::Vec2& size, const rw::math::Vec4& color);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

     private:
        std::shared_ptr<Renderer2D> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP
