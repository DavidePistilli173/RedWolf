//
// Created by david on 24/08/2025.
//

#include "camera.hpp"

rw::gfx::Camera rw::gfx::Camera::orthographic(float left, float right, float bottom, float top) {
    Camera camera;
    camera.prj_mat_ = rw::math::ortho(left, right, bottom, top, -1.0F, 1.0F);
    camera.recompute_view_matrix_();
    return camera;
}

const rw::math::Vec3& rw::gfx::Camera::position() const {
    return position_;
}

const rw::math::Mat4& rw::gfx::Camera::projection_matrix() const {
    return prj_mat_;
}

void rw::gfx::Camera::rotate(const float angle) {
    rotation_ += angle;
    recompute_view_matrix_();
}

float rw::gfx::Camera::rotation() const {
    return rotation_;
}

void rw::gfx::Camera::roto_translate(const rw::math::Vec3& delta, const float angle) {
    position_ += delta;
    rotation_ += angle;
    recompute_view_matrix_();
}

void rw::gfx::Camera::set_position(const rw::math::Vec3& position) {
    position_ = position;
    recompute_view_matrix_();
}

void rw::gfx::Camera::set_rotation(const float rotation) {
    rotation_ = rotation;
    recompute_view_matrix_();
}

void rw::gfx::Camera::translate(const rw::math::Vec3& delta) {
    position_ += delta;
    recompute_view_matrix_();
}

const rw::math::Mat4& rw::gfx::Camera::view_matrix() const {
    return view_mat_;
}

const rw::math::Mat4& rw::gfx::Camera::view_projection_matrix() const {
    return view_prj_mat_;
}

void rw::gfx::Camera::recompute_view_matrix_() {
    const rw::math::Mat4 transform{ rw::math::rotate(
        rw::math::translate(rw::math::Mat4(1.0F), position_), rw::math::radians(rotation_), rw::math::Vec3(0.0F, 0.0F, 1.0F)) };
    view_mat_     = rw::math::inverse(transform);
    view_prj_mat_ = prj_mat_ * view_mat_;
}