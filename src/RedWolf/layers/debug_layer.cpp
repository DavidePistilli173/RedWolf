module;

#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

module redwolf.layers.debug_layer;

import redwolf.engine.app;

void rw::layers::DebugLayer::attach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io{ ImGui::GetIO() };
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style{ ImGui::GetStyle() };
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    rw::ui::WindowHandle window{ rw::engine::App::get().window().handle() };

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
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