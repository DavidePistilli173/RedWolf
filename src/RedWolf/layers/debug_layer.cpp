//
// Created by cjm036653 on 14/08/2025.
//

#include "debug_layer.hpp"

#include "../engine/app.hpp"

#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

rw::layers::DebugLayer::DebugLayer() : Layer("DebugLayer") {}

void rw::layers::DebugLayer::attach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io{ ImGui::GetIO() };
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style{ ImGui::GetStyle() };
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void rw::layers::DebugLayer::begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void rw::layers::DebugLayer::detach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void rw::layers::DebugLayer::end_frame() {
    ImGuiIO&        io{ ImGui::GetIO() };
    rw::ui::Window& window{ rw::engine::App::get().window() };
    io.DisplaySize = ImVec2(static_cast<float>(window.width()), static_cast<float>(window.height()));

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (0 != (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)) {
        rw::ui::WindowHandle context_backup{ glfwGetCurrentContext() };
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(context_backup);
    }
}

/**
 * @brief Render ImGui UI for the layer.
 */
void rw::layers::DebugLayer::render_imgui() {
    static bool show{ true };
    ImGui::ShowDemoWindow(&show);
}

void rw::layers::DebugLayer::update([[maybe_unused]] const float delta_time) {}

bool rw::layers::DebugLayer::on_event([[maybe_unused]] const rw::evt::Event& event) {
    return false;
}
