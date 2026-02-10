module;

#include <future>

export module redwolf.gfx.renderer_interface_2_d;

import redwolf.common;
import redwolf.gfx.camera;
import redwolf.gfx.font;
import redwolf.gfx.framebuffer;
import redwolf.gfx.framebuffer_descriptor;
import redwolf.gfx.quad;
import redwolf.gfx.renderer_2_d;
import redwolf.gfx.shader;
import redwolf.gfx.text;
import redwolf.gfx.texture2d;
import redwolf.math;

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
         * @begin Bind a framebuffer for rendering.
         * @param framebuffer_handle Handle of the framebuffer to bind.
         */
        void bind_framebuffer(const Handle<Framebuffer> framebuffer_handle) const {
            renderer_->bind_framebuffer(framebuffer_handle);
        }

        /**
         * @brief Compute a sub-region of the texture.
         * @param texture_handle Handle to the texture.
         * @param region Region of the texture to compute, in pixels.
         * @return Texture coordinates of the sub-region.
         */
        [[nodiscard]] std::future<Texture2D::SubRegion>
            compute_texture_subregion(const Handle<Texture2D> texture_handle, const rw::math::Rect<float>& region) const {
            std::promise<Texture2D::SubRegion> promise;
            promise.set_value(renderer_->compute_texture_subregion(texture_handle, region));
            return promise.get_future();
        }

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen() {
            renderer_->clear_screen();
        }

        /**
         * @brief Clone the data of a single framebuffer.
         * @param framebuffer_handle Handle of the framebuffer to clone.
         * @return Framebuffer data.
         */
        [[nodiscard]] std::future<Framebuffer> clone_framebuffer(const Handle<Framebuffer> framebuffer_handle) {
            std::promise<Framebuffer> promise;
            promise.set_value(renderer_->clone_framebuffer(framebuffer_handle));
            return promise.get_future();
        }

        /**
         * @brief Create a framebuffer with the given descriptor.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] std::future<Handle<Framebuffer>> create_framebuffer(const FramebufferDescriptor& descriptor) {
            std::promise<Handle<Framebuffer>> promise;
            promise.set_value(renderer_->create_framebuffer(descriptor));
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
         * @brief Draw a string to the screen.
         * @param shader Shader to use for rendering.
         * @param text Text data to render.
         */
        void draw_text(rw::Handle<Shader> shader, const Text& text) {
            renderer_->draw_text(shader, text);
        }

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene() {
            renderer_->end_scene();
        }

        /**
         * @brief Get one of the default fonts.
         * @param font Font to get.
         * @return Handle to the specified font.
         */
        [[nodiscard]] std::future<Handle<Font>> get_default_font(const Renderer2D::DefaultFont font) {
            std::promise<Handle<Font>> promise;
            promise.set_value(renderer_->get_default_font(font));
            return promise.get_future();
        }

        /**
         * @brief Get one of the default shaders.
         * @param shader Shader to get.
         * @return Handle to the requested shader.
         */
        [[nodiscard]] std::future<Handle<Shader>> get_default_shader(const Renderer2D::DefaultShader shader) {
            std::promise<Handle<Shader>> promise;
            promise.set_value(renderer_->get_default_shader(shader));
            return promise.get_future();
        }

        /**
         * @brief Get one of the default textures.
         * @param texture Texture to get.
         * @return Handle to the requested texture.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> get_default_texture(const Renderer2D::DefaultTexture texture) {
            std::promise<Handle<Texture2D>> promise;
            promise.set_value(renderer_->get_default_texture(texture));
            return promise.get_future();
        }

        /**
         * @brief Load a texture from file.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] std::future<Handle<Texture2D>> load_texture(const std::string& file_path) {
            std::promise<Handle<Texture2D>> promise;
            promise.set_value(renderer_->load_texture(file_path));
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

        /**
         * @brief Unbind any currently bound framebuffer.
         */
        void unbind_framebuffer() const {
            renderer_->unbind_framebuffer();
        }

     private:
        std::shared_ptr<Renderer2D> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx
