module;

#include <glad/glad.h>
#include <span>
#include <stb/stb_image.h>
#include <string_view>

export module redwolf.gfx.texture2d_manager;

import redwolf.common;
import redwolf.core.asset_library;
import redwolf.core.geometry;
import redwolf.core.math;
import redwolf.gfx.common;
import redwolf.gfx.texture2d;
import redwolf.util.logger;

export namespace rw::gfx {
    /**
     * @brief Class for managing 2D textures.
     */
    class Texture2DManager {
     public:
        /**
         * @brief Default constructor.
         */
        Texture2DManager() = default;

        /**
         * @brief Destructor.
         */
        ~Texture2DManager() {
            textures_.remove_all([](Texture2D& texture) {
                glDeleteTextures(1, &texture.gid);
                texture.gid = 0;
            });

            rw::trace("All textures unloaded.");
        }

        /**
         * @brief Copy-constructor.
         */
        Texture2DManager(const Texture2DManager&) = delete;

        /**
         * @brief Move constructor.
         */
        Texture2DManager(Texture2DManager&&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Texture2DManager& operator=(const Texture2DManager&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        Texture2DManager& operator=(Texture2DManager&&) = delete;

        /**
         * @brief Bind a texture to the current context.
         * @param texture_handle Handle to the texture to bind.
         * @param slot Texture slot to bind to.
         */
        void bind(const Handle<Texture2D> texture_handle, const uint32_t slot) const {
            textures_.visit<void>(texture_handle, [slot](const Texture2D& texture) { glBindTextureUnit(slot, texture.gid); });
        }

        /**
         * @brief Compute a sub-region of the texture.
         * @param texture_handle Handle to the texture.
         * @param region Region of the texture to compute, in pixels.
         * @return Texture coordinates of the sub-region.
         */
        [[nodiscard]] rw::gfx::Texture2D::SubRegion
            compute_subregion(const Handle<Texture2D> texture_handle, const rw::core::Rect<float>& region) const {
            return textures_.visit<Texture2D::SubRegion>(texture_handle, [&region](const Texture2D& texture) {
                const float min_x{ region.x / static_cast<float>(texture.width) };
                const float max_x{ (region.x + region.width) / static_cast<float>(texture.width) };
                const float min_y{ region.y / static_cast<float>(texture.height) };
                const float max_y{ (region.y + region.height) / static_cast<float>(texture.height) };

                Texture2D::SubRegion result{};
                result[0] = { min_x, min_y }; // Bottom-left
                result[1] = { max_x, min_y }; // Bottom-right
                result[2] = { max_x, max_y }; // Top-right
                result[3] = { min_x, max_y }; // Top-left
                return result;
            });
        }

        /**
         * @brief Create a new empty texture.
         * @return Handle to the newly created texture.
         */
        [[nodiscard]] Handle<Texture2D> new_empty() {
            auto [texture, texture_handle] = textures_.create();

            glCreateTextures(GL_TEXTURE_2D, 1, &texture.gid);

            glTextureParameteri(texture.gid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(texture.gid, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(texture.gid, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture.gid, GL_TEXTURE_WRAP_T, GL_REPEAT);

            rw::trace("Texture {} (gid={}) created (empty).", texture_handle.index, texture.gid);
            return texture_handle;
        }

        /**
         * @brief Create a new texture from a file.
         * @param path Path to the file to load.
         * @return Handle to the newly created texture.
         */
        [[nodiscard]] Handle<Texture2D> new_from_path(const std::string_view path) {
            auto [texture, texture_handle] = textures_.create();

            texture.path = path;

            int32_t channels{ 0 };
            int32_t width{ 0 };
            int32_t height{ 0 };
            stbi_set_flip_vertically_on_load(1);
            stbi_uc* texture_data{ stbi_load(texture.path.c_str(), &width, &height, &channels, 0) };
            if (nullptr == texture_data) {
                rw::err("Failed to load image: {}", texture.path);
                textures_.remove(texture_handle, []([[maybe_unused]] Texture2D&) {});
                return {};
            }

            if (0 > width || 0 > height) {
                rw::err("Invalid image dimensions: {}x{} for image: {}", width, height, texture.path);
                stbi_image_free(texture_data);
                textures_.remove(texture_handle, []([[maybe_unused]] Texture2D&) {});
                return {};
            }
            texture.width  = static_cast<uint32_t>(width);
            texture.height = static_cast<uint32_t>(height);

            // Adjust the format depending on the loaded channels.
            if (4 == channels) {
                texture.internal_format = GL_RGBA8;
                texture.data_format     = GL_RGBA;
            } else if (3 == channels) {
                texture.internal_format = GL_RGB8;
                texture.data_format     = GL_RGB;
            } else if (1 == channels) {
                texture.internal_format = GL_R8;
                texture.data_format     = GL_RED;
            } else {
                rw::err("Unsupported number of channels: {} for image: {}", channels, texture.path);
                stbi_image_free(texture_data);
                textures_.remove(texture_handle, []([[maybe_unused]] Texture2D&) {});
                return {};
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &texture.gid);
            glTextureStorage2D(texture.gid, 1, texture.internal_format, width, height);

            glTextureParameteri(texture.gid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(texture.gid, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(texture.gid, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture.gid, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(texture.gid, 0, 0, 0, width, height, texture.data_format, GL_UNSIGNED_BYTE, texture_data);
            stbi_image_free(texture_data);

            rw::trace("Texture {} (gid={}) created from {}", texture_handle.index, texture.gid, texture.path);
            return texture_handle;
        }

        void set_data(const Handle<Texture2D> texture_handle, const std::span<const uint8_t> data, const TextureDescriptor& descriptor) {
            textures_.visit<void>(texture_handle, [texture_handle, data, &descriptor](Texture2D& texture) {
                texture.width  = descriptor.width;
                texture.height = descriptor.height;

                // Set the data format.
                if (!set_data_format_(texture, descriptor.format)) {
                    rw::err("Failed to set data format for texture (index:{},gid:{}).", texture_handle.index, texture.gid);
                    return;
                }

                if (data.size() < texture.width * texture.height * bytes_per_pixel_(texture.data_format)) {
                    rw::err(
                        "Insufficient data to fill the texture: size={}; width={}; height={}; bytes_per_pixel={}",
                        data.size(),
                        texture.width,
                        texture.height,
                        bytes_per_pixel_(texture.data_format));
                    return;
                }
                glTextureStorage2D(
                    texture.gid, 1, texture.internal_format, static_cast<GLsizei>(texture.width), static_cast<GLsizei>(texture.height));
                glTextureSubImage2D(
                    texture.gid,
                    0,
                    0,
                    0,
                    static_cast<int32_t>(texture.width),
                    static_cast<int32_t>(texture.height),
                    texture.data_format,
                    GL_UNSIGNED_BYTE,
                    data.data());
            });
        }

        /**
         * @brief Get the size of a given texture.
         * @param texture_handle Handle to the texture to get the size of.
         * @return Size of the texture. (0,0) if it wasn't found.
         */
        [[nodiscard]] rw::core::Vec2 texture_size(const Handle<Texture2D> texture_handle) const {
            return textures_.visit<rw::core::Vec2>(texture_handle, [](const Texture2D& texture) {
                return rw::core::Vec2{ static_cast<float>(texture.width), static_cast<float>(texture.height) };
            });
        }

        /**
         * @brief Unload a given texture.
         * @param handle Handle to the texture to unload.
         */
        void unload(const Handle<Texture2D> handle) {
            textures_.remove(handle, [](Texture2D& texture) {
                glDeleteTextures(1, &texture.gid);
                texture.gid = 0;
            });

            rw::trace("Texture {} unloaded.", handle.index);
        }

     private:
        /**
         * @brief Get the size of a pixel in bytes, given the texture's data format.
         * @param data_format Data format to get the pixel size for.
         * @return Pixel size with the given data format. [B]
         */
        [[nodiscard]] static constexpr uint32_t bytes_per_pixel_(const GLenum data_format) {
            switch (data_format) {
            case GL_RGB:
                return 3;
            case GL_RGBA:
                return 4;
            default:
                rw::err("Invalid data format: {}", data_format);
                return 4;
            }
        }

        /**
         * @brief Set the internal data format of a texture.
         * @param texture Texture to set the format data of.
         * @param format User-specified texture format.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool set_data_format_(Texture2D& texture, const TextureFormat format) {
            switch (format) {
            case TextureFormat::rgb8:
                texture.internal_format = GL_RGB8;
                texture.data_format     = GL_RGB;
                break;
            case TextureFormat::rgba8:
                texture.internal_format = GL_RGBA8;
                texture.data_format     = GL_RGBA;
                break;
            case TextureFormat::r8:
            case TextureFormat::rgba32f:
            default:
                rw::err("Unsupported texture format: {}", static_cast<int>(format));
                return false;
            }

            return true;
        }

        rw::core::AssetLibrary<Texture2D> textures_; /**< Actual storage for textures. */
    };
} // namespace rw::gfx
