module;

#include <RedWolf/macros.hpp>
#include <imgui/imgui.h>
#include <memory>
#include <optional>

export module sandbox.sandbox2d;

import redwolf;

static constexpr uint64_t texture_id{ rw::gfx::Renderer2D::max_reserved_texture_id + 1U };
static constexpr uint64_t spritesheet_id{ texture_id + 1U };

static constexpr uint64_t test_framebuffer_id{ rw::gfx::Renderer2D::max_reserved_framebuffer_id + 1U };

export class Sandbox2D : public rw::layers::Layer {
 public:
    Sandbox2D() : Layer("Sandbox2D"), camera_controller_{ 1280, 720, true, false, true } {
        renderer_interface_ = rw::engine::App::get().window().renderer_interface_2d();

        base_shader_ = renderer_interface_->get_shader(rw::gfx::Renderer2D::base_shader_id).get();
        if (!base_shader_.valid()) {
            RW_ERR("Failed to get shader {}", rw::gfx::Renderer2D::base_shader_id);
            return;
        }

        quad_1_ = { .position           = { -1.0F, 0.0F, 0.0F },
                    .rotation           = 0.0F,
                    .size               = { 0.8F, 0.8F },
                    .color              = {},
                    .tiling_factor      = 1.0F,
                    .texture            = rw::Handle<rw::gfx::Texture2D>{},
                    .texture_sub_region = std::nullopt };

        quad_2_ = { .position           = { 0.5F, -0.5F, 0.0F },
                    .rotation           = 0.0F,
                    .size               = { 0.5F, 0.75F },
                    .color              = { 0.2F, 0.3F, 0.8F, 1.0F },
                    .tiling_factor      = 1.0F,
                    .texture            = rw::Handle<rw::gfx::Texture2D>{},
                    .texture_sub_region = std::nullopt };

        quad_3_ = { .position           = { 0.5F, 0.5F, 0.0F },
                    .rotation           = 45.0F,
                    .size               = { 0.55F, 0.87F },
                    .color              = { 0.2F, 0.8F, 0.3F, 1.0F },
                    .tiling_factor      = 1.0F,
                    .texture            = rw::Handle<rw::gfx::Texture2D>{},
                    .texture_sub_region = std::nullopt };

        quad_4_ = { .position      = { 0.0F, 0.0F, -0.1F },
                    .rotation      = -30.0F,
                    .size          = { 100.0F, 100.0F },
                    .color         = { 0.2F, 0.2F, 0.8F, 1.0F },
                    .tiling_factor = 100.0F,
                    .texture =
                        renderer_interface_->load_texture(texture_id, "../src/RedWolfSandbox/assets/textures/checkerboard.png").get(),
                    .texture_sub_region = std::nullopt };

        quad_5_ = { .position           = { 0.0F, 0.0F, -0.05F },
                    .rotation           = 0.0F,
                    .size               = { 50.0F, 50.0F },
                    .color              = { 1.0F, 1.0F, 1.0F, 1.0F },
                    .tiling_factor      = 1.0F,
                    .texture            = quad_4_.texture,
                    .texture_sub_region = std::nullopt };

        if (!quad_4_.texture.valid()) {
            RW_ERR("Failed to get texture {}", texture_id);
            return;
        }

        spritesheet_quad_ = {
            .position      = { 0.0F, 0.0F, 0.0F },
            .rotation      = 0.0F,
            .size          = { 1.0F, 1.0F },
            .color         = { 1.0F, 1.0F, 1.0F, 1.0F },
            .tiling_factor = 1.0F,
            .texture = renderer_interface_->load_texture(spritesheet_id, "../src/RedWolfSandbox/assets/textures/rpg_spritesheet.png").get(),
            .texture_sub_region = std::nullopt
        };

        rgp_spritesheet_ = spritesheet_quad_.texture;
        if (!rgp_spritesheet_.valid()) {
            RW_ERR("Failed to get texture {}", spritesheet_id);
            return;
        }

        static constexpr rw::math::Rect<float> sub_region_rect{ .x = 256.0F, .y = 128.0F, .width = 128.0F, .height = 256.0F };
        spritesheet_quad_.texture_sub_region = spritesheet_quad_.texture->compute_sub_region(sub_region_rect);

        const rw::gfx::FramebufferDescriptor framebuffer_descriptor{ .width  = rw::engine::App::get().window().width(),
                                                                     .height = rw::engine::App::get().window().height() };
        test_framebuffer_ = renderer_interface_->create_framebuffer(test_framebuffer_id, framebuffer_descriptor).get();
        if (!test_framebuffer_.valid()) {
            RW_ERR("Failed to create framebuffer {}", test_framebuffer_id);
            return;
        }
    }

    ~Sandbox2D() override                  = default;
    Sandbox2D(const Sandbox2D&)            = delete;
    Sandbox2D& operator=(const Sandbox2D&) = delete;
    Sandbox2D(Sandbox2D&&)                 = default;
    Sandbox2D& operator=(Sandbox2D&&)      = default;

    void attach() override {}
    void detach() override {}

    void render_imgui() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(quad_1_.color));
        ImGui::End();

        ImGui::Begin("Test");
        uint32_t framebuffer_id{ test_framebuffer_->color_attachment_id() };
        ImGui::Image(framebuffer_id, ImVec2{ 320.0F, 160.0F });
        ImGui::End();

        ImGui::Begin("Profiling");
        ImGui::Text(
            "min=%.3fms; last=%.3fms; max=%.3fms | %s",
            update_timing_.elapsed_min_milliseconds(),
            update_timing_.elapsed_last_milliseconds(),
            update_timing_.elapsed_max_milliseconds(),
            update_timing_.name.data());
        ImGui::Text(
            "min=%.3fms; last=%.3fms; max=%.3fms | %s",
            render_timing_.elapsed_min_milliseconds(),
            render_timing_.elapsed_last_milliseconds(),
            render_timing_.elapsed_max_milliseconds(),
            render_timing_.name.data());

        const auto stats{ renderer_interface_->stats().get() };
        ImGui::Text("Renderer stats: %u draw calls, %u quads", stats.draw_calls, stats.quad_count);
        ImGui::End();
    }

    void update(const float delta_time) override {
        const rw::time::Profiler profiler{ update_timing_ };

        // Update
        camera_controller_.update(delta_time);
        static float rotation{ 0.0F };
        rotation += 2.0F * delta_time;
        quad_3_.rotation = rotation;

        // Render
        {
            const rw::time::Profiler renderer_profiler{ render_timing_ };
            renderer_interface_->reset_stats();

            renderer_interface_->clear_screen();

            test_framebuffer_->bind();
            renderer_interface_->clear_screen();
            {
                renderer_interface_->begin_scene(camera_controller_.camera());
                renderer_interface_->draw_quad(base_shader_, quad_4_);
                renderer_interface_->draw_quad(base_shader_, quad_5_);
                renderer_interface_->draw_quad(base_shader_, quad_1_);
                renderer_interface_->draw_quad(base_shader_, quad_2_);
                renderer_interface_->draw_quad(base_shader_, quad_3_);
                renderer_interface_->end_scene();
            }

            {
                renderer_interface_->begin_scene(camera_controller_.camera());
                for (float y{ -5.0F }; y < 5.0F; y += 0.5F) {
                    for (float x{ -5.0F }; x < 5.0F; x += 0.5F) {
                        rw::gfx::Quad quad{ .position           = { x, y, 0.1F },
                                            .rotation           = 0.0F,
                                            .size               = { 0.45F, 0.45F },
                                            .color              = { (x + 5.0F) / 10.0F, 0.4F, (y + 5.0F) / 10.0F, 0.7F },
                                            .tiling_factor      = 1.0F,
                                            .texture            = rw::Handle<rw::gfx::Texture2D>{},
                                            .texture_sub_region = std::nullopt };
                        renderer_interface_->draw_quad(base_shader_, quad);
                    }
                }
                renderer_interface_->end_scene();
            }
            test_framebuffer_->unbind();

            {
                renderer_interface_->begin_scene(camera_controller_.camera());
                renderer_interface_->draw_quad(base_shader_, spritesheet_quad_);
                renderer_interface_->end_scene();
            }
        }
    }

    [[nodiscard]] bool on_event(const rw::evt::Event& event) override {
        return camera_controller_.on_event(event);
    }

 private:
    rw::engine::CameraController                  camera_controller_;
    std::unique_ptr<rw::gfx::RendererInterface2D> renderer_interface_; /**< Interface to the renderer. */

    rw::Handle<rw::gfx::Shader> base_shader_;

    rw::gfx::Quad quad_1_;
    rw::gfx::Quad quad_2_;
    rw::gfx::Quad quad_3_;
    rw::gfx::Quad quad_4_;
    rw::gfx::Quad quad_5_;

    rw::Handle<rw::gfx::Texture2D> rgp_spritesheet_;
    rw::gfx::Quad                  spritesheet_quad_;

    rw::Handle<rw::gfx::Framebuffer> test_framebuffer_;

    rw::time::ProfilerData update_timing_{ "Sandbox2D::update" };
    rw::time::ProfilerData render_timing_{ "Sandbox2D::update - render" };
};
