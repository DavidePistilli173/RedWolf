module;

#include <cstdint>
#include <string>

export module redwolf.gfx.shader;

export namespace rw::gfx {
    /**
     * @brief OpenGL shader class.
     */
    struct Shader {
        uint32_t gid{ 0U }; /**< ID of the shader. */
    };
} // namespace rw::gfx
