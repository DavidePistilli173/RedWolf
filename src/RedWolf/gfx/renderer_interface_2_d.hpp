//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP
#define SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP

#include "gfx.hpp"
#include "renderer_2_d.hpp"

#include <future>

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
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen();

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order..
         * @param shader Shader to use for rendering.
         * @param transform Transformation matrix of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(Shader* shader, const rw::math::Mat4& transform, const rw::math::Vec4& color);

        /**
         * @brief Draw a textured quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param transform Transformation matrix of the quad.
         * @param texture Texture for the quad.
         */
        void draw_quad(Shader* shader, const rw::math::Mat4& transform, Texture2D* texture);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

        /**
         * @brief Get a shader that was previously loaded by the renderer.
         * @param id ID of the loaded shader.
         * @return Pointer to the loaded shader, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Shader*> get_shader(const uint64_t id);

        /**
         * @brief Get a texture that was previously loaded by the renderer.
         * @param id ID of the loaded texture.
         * @return Pointer to the loaded texture, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Texture2D*> get_texture(const uint64_t id);

        /**
         * @brief Load a texture from file.
         * @param id ID of the texture to load. If the ID already exists, the texture is replaced.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] std::future<Texture2D*> load_texture(const uint64_t id, const std::string& file_path);

     private:
        std::shared_ptr<Renderer2D> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP
