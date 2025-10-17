//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_SANDBOX_2_D_HPP
#define SRC_REDWOLF_SANDBOX_2_D_HPP

#include <RedWolf/engine/camera_controller.hpp>
#include <RedWolf/gfx/renderer_interface_2_d.hpp>
#include <RedWolf/layers/layer.hpp>
#include <RedWolf/time/profiler_data.hpp>

class Sandbox2D : public rw::layers::Layer {
 public:
    Sandbox2D();
    ~Sandbox2D() override                  = default;
    Sandbox2D(const Sandbox2D&)            = delete;
    Sandbox2D& operator=(const Sandbox2D&) = delete;
    Sandbox2D(Sandbox2D&&)                 = default;
    Sandbox2D& operator=(Sandbox2D&&)      = default;

    void               attach() override;
    void               detach() override;
    void               render_imgui() override;
    void               update(const float delta_time) override;
    [[nodiscard]] bool on_event(const rw::evt::Event& event) override;

 private:
    rw::engine::CameraController                  camera_controller_;
    std::unique_ptr<rw::gfx::RendererInterface2D> renderer_interface_; /**< Interface to the renderer. */

    rw::gfx::Shader* base_shader_{ nullptr };

    rw::gfx::Quad quad_1_;
    rw::gfx::Quad quad_2_;
    rw::gfx::Quad quad_3_;
    rw::gfx::Quad quad_4_;
    rw::gfx::Quad quad_5_;

    rw::gfx::Texture2D* rgp_spritesheet_{ nullptr };
    rw::gfx::Quad       spritesheet_quad_;

    rw::gfx::Framebuffer* test_framebuffer_{ nullptr };

    rw::time::ProfilerData update_timing_{ "Sandbox2D::update" };
    rw::time::ProfilerData render_timing_{ "Sandbox2D::update - render" };
};

#endif // SRC_REDWOLF_SANDBOX_2_D_HPP
