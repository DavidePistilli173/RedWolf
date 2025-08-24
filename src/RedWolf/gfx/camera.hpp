//
// Created by david on 24/08/2025.
//

#ifndef SRC_REDWOLF_CAMERA_HPP
#define SRC_REDWOLF_CAMERA_HPP

#include "RedWolf/math/math.hpp"

namespace rw::gfx {
    /**
     * @brief Class representing a camera in a 3D environment.
     */
    class Camera {
     public:
        /**
         * @brief Constructor.
         */
        Camera() = default;

        /**
         * @brief Create an orthographic camera.
         * @param left Left limit of the frustum.
         * @param right Right limit of the frustum.
         * @param bottom Bottom limit of the frustum.
         * @param top Top limit of the frustum.
         * @return New orthographic camera.
         */
        [[nodiscard]] static Camera orthographic(float left, float right, float bottom, float top);

        /**
         * @brief Get the camera position.
         * @return Camera position in world space.
         */
        [[nodiscard]] const rw::math::Vec3& position() const;

        /**
         * @brief Get the projection matrix of the camera.
         * @return Projection matrix.
         */
        [[nodiscard]] const rw::math::Mat4& projection_matrix() const;

        /**
         * @brief Rotate the camera by a given angle around the Z axis in degrees.
         * @param angle Angle in degrees.
         */
        void rotate(const float angle);

        /**
         * @brief Get the camera rotation.
         * @return Camera rotation around the Z axis.
         */
        [[nodiscard]] float rotation() const;

        /**
         * @brief Perform a roto-translation of the camera.
         * @param delta Translation vector in world space.
         * @param angle Rotation angle in degrees around the Z axis.
         */
        void roto_translate(const rw::math::Vec3& delta, const float angle);

        /**
         * @brief Set a new camera position in world space.
         * @param position New camera position.
         */
        void set_position(const rw::math::Vec3& position);

        /**
         * @brief Set the camera rotation around the Z axis in degrees.
         * @param rotation New rotation angle in degrees.
         */
        void set_rotation(const float rotation);

        /**
         * @brief Move the camera by a given delta in world space.
         * @param delta Translation vector.
         */
        void translate(const rw::math::Vec3& delta);

        /**
         * @brief Get the view matrix of the camera.
         * @return View matrix.
         */
        [[nodiscard]] const rw::math::Mat4& view_matrix() const;

        /**
         * @brief Get the combined view-projection matrix.
         * @return Combined view-projection matrix.
         */
        [[nodiscard]] const rw::math::Mat4& view_projection_matrix() const;

     private:
        /**
         * @brief Recompute the view matrix and the combined view-projection matrix.
         */
        void recompute_view_matrix_();

        rw::math::Mat4 prj_mat_{ 1.0F };      /**< Projection matrix of the camera. */
        rw::math::Mat4 view_mat_{ 1.0F };     /**< View matrix of the camera. */
        rw::math::Mat4 view_prj_mat_{ 1.0F }; /**< Combined view-projection matrix of the camera. */

        rw::math::Vec3 position_{ 0.0F }; /**< Position of the camera in world space. */
        float          rotation_{ 0.0F }; /**< Rotation of the camera in degrees in the Z axis. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_CAMERA_HPP
