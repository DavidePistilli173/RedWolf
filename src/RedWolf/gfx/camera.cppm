module;

export module redwolf.gfx.camera;

import redwolf.math;

export namespace rw::gfx {
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
        [[nodiscard]] static Camera orthographic(const float left, const float right, const float bottom, const float top) {
            Camera camera;
            camera.prj_mat_ = rw::math::ortho(left, right, bottom, top, -1.0F, 1.0F);
            camera.recompute_view_matrix_();
            return camera;
        }

        /**
         * @brief Get the camera position.
         * @return Camera position in world space.
         */
        [[nodiscard]] const rw::math::Vec3& position() const {
            return position_;
        }

        /**
         * @brief Get the projection matrix of the camera.
         * @return Projection matrix.
         */
        [[nodiscard]] const rw::math::Mat4& projection_matrix() const {
            return prj_mat_;
        }

        /**
         * @brief Rotate the camera by a given angle around the Z axis in degrees.
         * @param angle Angle in degrees.
         */
        void rotate(const float angle) {
            rotation_ += angle;
            recompute_view_matrix_();
        }

        /**
         * @brief Get the camera rotation.
         * @return Camera rotation around the Z axis.
         */
        [[nodiscard]] float rotation() const {
            return rotation_;
        }

        /**
         * @brief Perform a roto-translation of the camera.
         * @param delta Translation vector in world space.
         * @param angle Rotation angle in degrees around the Z axis.
         */
        void roto_translate(const rw::math::Vec3& delta, const float angle) {
            position_ += delta;
            rotation_ += angle;
            recompute_view_matrix_();
        }

        /**
         * @brief Set a new camera position in world space.
         * @param position New camera position.
         */
        void set_position(const rw::math::Vec3& position) {
            position_ = position;
            recompute_view_matrix_();
        }

        /**
         * @brief Set a new orthographic projection.
         * @param left Left limit of the frustum.
         * @param right Right limit of the frustum.
         * @param bottom Bottom limit of the frustum.
         * @param top Top limit of the frustum.
         */
        void set_ortho_projection(const float left, const float right, const float bottom, const float top) {
            prj_mat_ = rw::math::ortho(left, right, bottom, top, -1.0F, 1.0F);
            recompute_view_matrix_();
        }

        /**
         * @brief Set the camera rotation around the Z axis in degrees.
         * @param rotation New rotation angle in degrees.
         */
        void set_rotation(const float rotation) {
            rotation_ = rotation;
            recompute_view_matrix_();
        }

        /**
         * @brief Move the camera by a given delta in world space.
         * @param delta Translation vector.
         */
        void translate(const rw::math::Vec3& delta) {
            position_ += delta;
            recompute_view_matrix_();
        }

        /**
         * @brief Get the view matrix of the camera.
         * @return View matrix.
         */
        [[nodiscard]] const rw::math::Mat4& view_matrix() const {
            return view_mat_;
        }

        /**
         * @brief Get the combined view-projection matrix.
         * @return Combined view-projection matrix.
         */
        [[nodiscard]] const rw::math::Mat4& view_projection_matrix() const {
            return view_prj_mat_;
        }

     private:
        /**
         * @brief Recompute the view matrix and the combined view-projection matrix.
         */
        void recompute_view_matrix_() {
            const rw::math::Mat4 transform{ rw::math::rotate(
                rw::math::translate(rw::math::Mat4(1.0F), position_), rw::math::radians(rotation_), rw::math::Vec3(0.0F, 0.0F, 1.0F)) };
            view_mat_     = rw::math::inverse(transform);
            view_prj_mat_ = prj_mat_ * view_mat_;
        }

        rw::math::Mat4 prj_mat_{ 1.0F };      /**< Projection matrix of the camera. */
        rw::math::Mat4 view_mat_{ 1.0F };     /**< View matrix of the camera. */
        rw::math::Mat4 view_prj_mat_{ 1.0F }; /**< Combined view-projection matrix of the camera. */

        rw::math::Vec3 position_{ 0.0F }; /**< Position of the camera in world space. */
        float          rotation_{ 0.0F }; /**< Rotation of the camera in degrees in the Z axis. */
    };
} // namespace rw::gfx
