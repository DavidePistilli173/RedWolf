module;

#include <cstdint>
#include <glad/glad.h>

export module redwolf.gfx.framebuffer_manager;

import redwolf.core.handle;
import redwolf.core.asset_library;
import redwolf.gfx.framebuffer;
import redwolf.gfx.framebuffer_descriptor;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class for managing framebuffers.
     */
    class FramebufferManager {
     public:
        /**
         * @brief Constructor.
         */
        FramebufferManager() = default;

        /**
         * @brief Destructor.
         */
        ~FramebufferManager() {
            framebuffers_.remove_all([](const Framebuffer& framebuffer) {
                glDeleteFramebuffers(1, &framebuffer.gid);
                glDeleteTextures(1, &framebuffer.color_attachment);
                glDeleteTextures(1, &framebuffer.depth_attachment);
            });
        }

        /**
         * @brief Copy constructor.
         */
        FramebufferManager(const FramebufferManager&) = delete;

        /**
         * @brief Move constructor.
         */
        FramebufferManager(FramebufferManager&&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        FramebufferManager& operator=(const FramebufferManager&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        FramebufferManager& operator=(FramebufferManager&&) = delete;

        /**
         * @brief Bind a framebuffer for rendering.
         * @param framebuffer_handle Handle to the framebuffer to bind.
         */
        void bind(const Handle<Framebuffer> framebuffer_handle) const {
            framebuffers_.visit<void>(framebuffer_handle, [](const Framebuffer& framebuffer) {
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.gid);
                glViewport(0, 0, static_cast<int32_t>(framebuffer.descriptor.width), static_cast<int32_t>(framebuffer.descriptor.height));
            });
        }

        /**
         * @brief Clone the data of a single framebuffer.
         * @param framebuffer_handle Handle of the framebuffer to clone.
         * @return Framebuffer data.
         */
        [[nodiscard]] Framebuffer clone_framebuffer(const Handle<Framebuffer> framebuffer_handle) {
            const auto res{ framebuffers_.unsafe_get_raw(framebuffer_handle) };
            if (!res.has_value()) {
                return {};
            }
            return res.value().get();
        }

        /**
         * @brief Create a new framebuffer from a descriptor.
         * @param descriptor Descriptor containing the framebuffer settings.
         * @return Handle to the newly created framebuffer.
         */
        [[nodiscard]] Handle<Framebuffer> new_from_descriptor(const FramebufferDescriptor& descriptor) {
            auto [framebuffer, framebuffer_handle]{ framebuffers_.create() };
            framebuffer.descriptor = descriptor;
            if (!recreate_internal_(framebuffer)) {
                rw::err("Failed to create framebuffer.");
                return {};
            }
            return framebuffer_handle;
        }

        /**
         * @brief Re-create the given framebuffer maintaining its settings.
         * @param framebuffer_handle Handle to the framebuffer to recreate.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool recreate(const Handle<Framebuffer> framebuffer_handle) {
            return framebuffers_.visit<bool>(framebuffer_handle, [](Framebuffer& framebuffer) { return recreate_internal_(framebuffer); });
        }

        /**
         * @brief Resize the framebuffer.
         * @param framebuffer_handle Handle to the framebuffer to resize.
         * @param width New framebuffer width.
         * @param height New framebuffer height.
         */
        [[nodiscard]] bool resize(const Handle<Framebuffer> framebuffer_handle, const uint32_t width, const uint32_t height) {
            return framebuffers_.visit<bool>(framebuffer_handle, [width, height](Framebuffer& framebuffer) {
                framebuffer.descriptor.width  = width;
                framebuffer.descriptor.height = height;
                if (!recreate_internal_(framebuffer)) {
                    rw::err("Failed to resize framebuffer {} to {}x{}.", framebuffer.gid, width, height);
                    return false;
                }
                return true;
            });
        }

        /**
         * @brief Unbind the current framebuffer.
         */
        void unbind() const {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

     private:
        /**
         * @brief Re-create the framebuffer with the current descriptor settings.
         * @param framebuffer Framebuffer to operate on.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool recreate_internal_(Framebuffer& framebuffer) {
            if (0 != framebuffer.gid) {
                glDeleteFramebuffers(1, &framebuffer.gid);
                glDeleteTextures(1, &framebuffer.color_attachment);
                glDeleteTextures(1, &framebuffer.depth_attachment);
                rw::trace("Framebuffer {} deleted.", framebuffer.gid);

                framebuffer.gid              = 0;
                framebuffer.color_attachment = 0;
                framebuffer.depth_attachment = 0;
            }

            glCreateFramebuffers(1, &framebuffer.gid);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.gid);

            // Create the colour attachment.
            glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer.color_attachment);
            glBindTexture(GL_TEXTURE_2D, framebuffer.color_attachment);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA8,
                static_cast<int>(framebuffer.descriptor.width),
                static_cast<int>(framebuffer.descriptor.height),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.color_attachment, 0);

            // Create the depth attachment.
            glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer.depth_attachment);
            glBindTexture(GL_TEXTURE_2D, framebuffer.depth_attachment);
            glTexStorage2D(
                GL_TEXTURE_2D,
                1,
                GL_DEPTH24_STENCIL8,
                static_cast<int32_t>(framebuffer.descriptor.width),
                static_cast<int32_t>(framebuffer.descriptor.height));

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, framebuffer.depth_attachment, 0);

            if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
                rw::err("Framebuffer {} is incomplete!", framebuffer.gid);
                return false;
            }

            // Unbind the framebuffer.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return true;
        }

        rw::core::AssetLibrary<Framebuffer> framebuffers_; /**< Actual storage for framebuffers. */
    };
} // namespace rw::gfx