module;

#include <cstdint>

export module redwolf.gfx.framebuffer;

import redwolf.gfx.framebuffer_descriptor;

export namespace rw::gfx {
    /**
     * @brief OpenGL Framebuffer implementation.
     */
    class Framebuffer {
     public:
        /**
         * @brief Constructor.
         * @param descriptor Framebuffer creation options.
         */
        explicit Framebuffer(const rw::gfx::FramebufferDescriptor& descriptor);

        /**
         * @brief Destructor.
         */
        ~Framebuffer();

        /**
         * @brief Copy constructor.
         */
        Framebuffer(const Framebuffer&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Framebuffer& operator=(const Framebuffer&) = delete;

        /**
         * @brief Move constructor.
         */
        Framebuffer(Framebuffer&& other);

        /**
         * @brief Move-assignment operator.
         */
        Framebuffer& operator=(Framebuffer&& other);

        /**
         * @brief Bind the framebuffer for rendering.
         */
        void bind() const;

        /**
         * @brief Get the raw framebuffer ID.
         * @return Raw framebuffer ID.
         */
        [[nodiscard]] uint32_t id() const;

        /**
         * @brief Get the renderer ID of the framebuffer's colour attachment.
         * @return Renderer ID of the colour attachment texture.
         */
        [[nodiscard]] uint32_t color_attachment_id() const;

        /**
         * @brief Get the current framebuffer settings.
         * @return Framebuffer settings descriptor.
         */
        [[nodiscard]] const rw::gfx::FramebufferDescriptor& descriptor() const;

        /**
         * @brief Re-create the framebuffer with the current descriptor settings.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool recreate();

        /**
         * @brief Resize the framebuffer.
         * @param width New framebuffer width.
         * @param height New framebuffer height.
         */
        void resize(uint32_t width, uint32_t height);

        /**
         * @brief Unbind the framebuffer.
         */
        void unbind();

     private:
        rw::gfx::FramebufferDescriptor descriptor_; /**< Current framebuffer settings. */

        uint32_t id_{ 0U };               /**< OpenGL object ID. */
        uint32_t color_attachment_{ 0U }; /**< ID of the colour attachment texture for the framebuffer. */
        uint32_t depth_attachment_{ 0U }; /**< ID of the depth attachment texture for the framebuffer. */
    };
} // namespace rw::gfx
