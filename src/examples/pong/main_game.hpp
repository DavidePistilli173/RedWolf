//
// Created by david on 21/12/2025.
//

#ifndef SRC_REDWOLF_MAIN_GAME_HPP
#define SRC_REDWOLF_MAIN_GAME_HPP

#include <RedWolf/engine/app.hpp>
#include <RedWolf/engine/camera_controller.hpp>
#include <RedWolf/gfx/renderer_interface_2_d.hpp>
#include <RedWolf/layers/layer.hpp>
#include <RedWolf/ui/window.hpp>

class MainGame : public rw::layers::Layer {
 public:
    static constexpr rw::math::Vec2 default_ball_velocity{ 400.0F, 200.0F };

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
    void update_ball_(const float delta_time);
    void update_logic_(const float delta_time);
    void update_paddles_(const float delta_time);
    void render_();

    rw::engine::CameraController                  camera_controller_; /**< Camera controller for 2D orthographic camera. */
    rw::ui::Window&                               window_{ rw::engine::App::get().window() }; /**< Reference to the application window. */
    std::unique_ptr<rw::gfx::RendererInterface2D> renderer_interface_;                        /**< Interface to the renderer. */
    rw::Handle<rw::gfx::Shader>                   base_shader_; /**< Base shader for rendering textured quads. */

    rw::gfx::Quad background_quad_; /**< Quad for the background. */
    rw::gfx::Quad mid_field_quad_;  /**< Quad for the mid-field line. */
    rw::gfx::Quad paddle_1_quad_;   /**< Quad for the first paddle. */
    rw::gfx::Quad paddle_2_quad_;   /**< Quad for the second paddle. */
    rw::gfx::Quad ball_quad_;       /**< Quad for the ball. */

    rw::math::Vec2 ball_velocity_{ default_ball_velocity }; /**< Current velocity of the ball. */
};

#endif // SRC_REDWOLF_MAIN_GAME_HPP
