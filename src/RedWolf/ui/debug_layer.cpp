//
// Created by cjm036653 on 14/08/2025.
//

#include "debug_layer.hpp"

#include "../engine/app.hpp"
#include "../pltf/gl/imgui_impl_opengl3.h"
#include "../util/logger.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

rw::ui::DebugLayer::DebugLayer(const ID id) : Layer(id, "DebugLayer") {}

void rw::ui::DebugLayer::attach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io{ ImGui::GetIO() };
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void rw::ui::DebugLayer::detach() {}

void rw::ui::DebugLayer::update() {
    rw::engine::App& app{ rw::engine::App::get() };
    ImGuiIO&         io{ ImGui::GetIO() };
    io.DisplaySize = ImVec2(static_cast<float>(app.window().width()), static_cast<float>(app.window().height()));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    const double time{ glfwGetTime() };
    io.DeltaTime = static_cast<float>(time_ > 0.0 ? time - time_ : 1.0 / 60.0);
    time_        = time;

    static bool show{ true };
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool rw::ui::DebugLayer::on_event([[maybe_unused]] const rw::evt::Event& event) {
    return false;
}