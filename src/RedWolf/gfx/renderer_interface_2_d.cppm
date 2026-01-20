module;

#include <future>

export module redwolf.gfx.renderer_interface_2_d;

import redwolf.common;
import redwolf.gfx.camera;
import redwolf.gfx.framebuffer;
import redwolf.gfx.framebuffer_descriptor;
import redwolf.gfx.quad;
import redwolf.gfx.renderer_2_d;
import redwolf.gfx.shader;
import redwolf.gfx.texture_2d;

export namespace rw::gfx {
    /**
     * @brief Interface for the strictly 2D renderer.
     */
    class RendererInterface2D {
     public:
        /**
         * @brief Constructor.
         * @param renderer Renderer instance to connect to.
         */
        explicit RendererInterface2D(std::shared_ptr<Renderer2D> renderer) : renderer_{ std::move(renderer) } {}

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera) {
            renderer_->begin_scene(camera);
        }

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen() {
            renderer_->clear_screen();
        }

        /**
         * @brief Create a framebuffer with the given descriptor.
         * @param id ID of the framebuffer to create.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] std::future<Handle<Framebuffer>> create_framebuffer(const uint64_t id, const FramebufferDescriptor& descriptor) {
            std::promise<Handle<Framebuffer>> promise;
            promise.set_value(renderer_->create_framebuffer(id, descriptor));
            return promise.get_future();
        }

        /**
         * @brief Draw a textured quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param quad Quad data to render.
         */
        void draw_quad(rw::Handle<Shader> shader, const Quad& quad) {
            renderer_->draw_quad(shader, quad);
        }

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene() {
            renderer_->end_scene();
        }

        /**
         * @brief Get a shader that was previously loaded by the renderer.
         * @param id ID of the loaded shader.
         * @return Pointer to the loaded shader, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Handle<Shader>> get_shader(const uint64_t id) {
            std::promise<Handle<Shader>> promise;
            promise.set_value(renderer_->get_shader(id));
            return promise.get_future();
        }

        /**
         * @brief Get a texture that was previously loaded by the renderer.
         * @param id ID of the loaded texture.
         * @return Pointer to the loaded texture, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> get_texture(const uint64_t id) {
            std::promise<Handle<Texture2D>> promise;
            promise.set_value(renderer_->get_texture(id));
            return promise.get_future();
        }

        /**
         * @brief Load a texture from file.
         * @param id ID of the texture to load. If the ID already exists, the texture is replaced.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> load_texture(const uint64_t id, const std::string& file_path) {
            std::promise<Handle<Texture2D>> promise;
            promise.set_value(renderer_->load_texture(id, file_path));
            return promise.get_future();
        }

        /**
         * @brief Reset the renderer statistics.
         */
        void reset_stats() {
            renderer_->reset_stats();
        }

        /**
         * @brief Set a new viewport for rendering.
         * @param x X coordinate of the bottom-left corner of the viewport.
         * @param y Y coordinate of the bottom-left corner of the viewport.
         * @param width New width.
         * @param height New height.
         */
        void set_viewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
            renderer_->set_viewport(x, y, width, height);
        }

        /**
         * @brief Get the current renderer statistics.
         * @return Current renderer statistics.
         */
        [[nodiscard]] std::future<Renderer2DStats> stats() const {
            std::promise<Renderer2DStats> promise;
            promise.set_value(renderer_->stats());
            return promise.get_future();
        }

     private:
        std::shared_ptr<Renderer2D> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx
