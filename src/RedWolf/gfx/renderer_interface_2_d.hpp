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
         * @brief Create a framebuffer with the given descriptor.
         * @param id ID of the framebuffer to create.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] std::future<Handle<Framebuffer>> create_framebuffer(const uint64_t id, const FramebufferDescriptor& descriptor);

        /**
         * @brief Draw a textured quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param quad Quad data to render.
         */
        void draw_quad(rw::Handle<Shader> shader, const Quad& quad);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

        /**
         * @brief Get a shader that was previously loaded by the renderer.
         * @param id ID of the loaded shader.
         * @return Pointer to the loaded shader, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Handle<Shader>> get_shader(const uint64_t id);

        /**
         * @brief Get a texture that was previously loaded by the renderer.
         * @param id ID of the loaded texture.
         * @return Pointer to the loaded texture, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> get_texture(const uint64_t id);

        /**
         * @brief Load a texture from file.
         * @param id ID of the texture to load. If the ID already exists, the texture is replaced.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> load_texture(const uint64_t id, const std::string& file_path);

        /**
         * @brief Reset the renderer statistics.
         */
        void reset_stats();

        /**
         * @brief Set a new viewport for rendering.
         * @param x X coordinate of the bottom-left corner of the viewport.
         * @param y Y coordinate of the bottom-left corner of the viewport.
         * @param width New width.
         * @param height New height.
         */
        void set_viewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);

        /**
         * @brief Get the current renderer statistics.
         * @return Current renderer statistics.
         */
        [[nodiscard]] std::future<Renderer2DStats> stats() const;

     private:
        std::shared_ptr<Renderer2D> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_INTERFACE_2_D_HPP
