//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_SANDBOX_2_D_HPP
#define SRC_REDWOLF_SANDBOX_2_D_HPP

#include <RedWolf/engine/camera_controller.hpp>
#include <RedWolf/gfx/renderer_interface_2_d.hpp>
#include <RedWolf/layers/layer.hpp>

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

    std::shared_ptr<rw::gfx::VertexArray> square_va_;
    rw::gfx::Shader*                      shader_{ nullptr };
    rw::math::Vec4                        square_color_{ 1.0F };
};

#endif // SRC_REDWOLF_SANDBOX_2_D_HPP
