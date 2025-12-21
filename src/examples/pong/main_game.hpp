//
// Created by david on 21/12/2025.
//

#ifndef SRC_REDWOLF_MAIN_GAME_HPP
#define SRC_REDWOLF_MAIN_GAME_HPP

#include <RedWolf/engine/camera_controller.hpp>
#include <RedWolf/gfx/renderer_interface_2_d.hpp>
#include <RedWolf/layers/layer.hpp>

class MainGame : public rw::layers::Layer {
 public:
    MainGame();
    ~MainGame() override                 = default;
    MainGame(const MainGame&)            = delete;
    MainGame& operator=(const MainGame&) = delete;
    MainGame(MainGame&&)                 = default;
    MainGame& operator=(MainGame&&)      = default;

    void               attach() override;
    void               detach() override;
    void               render_imgui() override;
    void               update(const float delta_time) override;
    [[nodiscard]] bool on_event(const rw::evt::Event& event) override;

 private:
    void update_logic_(const float delta_time);
    void render_();

    rw::engine::CameraController                  camera_controller_;  /**< Camera controller for 2D orthographic camera. */
    std::unique_ptr<rw::gfx::RendererInterface2D> renderer_interface_; /**< Interface to the renderer. */
    rw::Handle<rw::gfx::Shader>                   base_shader_;        /**< Base shader for rendering textured quads. */

    rw::gfx::Quad background_quad_; /**< Quad for the background. */
};

#endif // SRC_REDWOLF_MAIN_GAME_HPP
