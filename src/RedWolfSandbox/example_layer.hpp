//
// Created by david on 24/08/2025.
//

#ifndef SRC_REDWOLF_EXAMPLE_LAYER_HPP
#define SRC_REDWOLF_EXAMPLE_LAYER_HPP

#include <RedWolf/gfx/camera.hpp>
#include <RedWolf/gfx/renderer_interface.hpp>
#include <RedWolf/layers/layer.hpp>

class ExampleLayer : public rw::layers::Layer {
 public:
    ExampleLayer();

    /**
     * @brief Attach to the layer stack.
     */
    void attach() override;

    /**
     * @brief Detach from the layer stack.
     */
    void detach() override;

    /**
     * @brief Render ImGui UI for the layer.
     */
    void render_imgui() override;

    /**
     * @brief Update the layer (called each frame).
     */
    void update() override;

    /**
     * @brief Event handling function.
     * @param event Event to handle.
     */
    [[nodiscard]] bool on_event(const rw::evt::Event& event) override;

 private:
    rw::gfx::Camera                             camera_;
    std::unique_ptr<rw::gfx::RendererInterface> renderer_interface_; /**< Interface to the renderer. */

    std::unique_ptr<rw::gfx::Shader>      shader_;
    std::unique_ptr<rw::gfx::VertexArray> vertex_array_;
    std::unique_ptr<rw::gfx::VertexArray> square_va_;

    float speed_x{ 0.0F };
    float speed_y{ 0.0F };
    float rotation_speed_{ 0.0F };
};

#endif // SRC_REDWOLF_EXAMPLE_LAYER_HPP
