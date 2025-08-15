//
// Created by cjm036653 on 14/08/2025.
//

#include "debug_layer.hpp"

#include "../engine/app.hpp"
#include "../input/polling.hpp"
#include "../pl/gl/imgui_impl_opengl3.h"
#include "../util/logger.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui/imgui.h>

rw::layers::DebugLayer::DebugLayer(const ID id) : Layer(id, "DebugLayer") {}

void rw::layers::DebugLayer::attach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io{ ImGui::GetIO() };
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void rw::layers::DebugLayer::detach() {}

void rw::layers::DebugLayer::update() {
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

bool rw::layers::DebugLayer::on_event(const rw::evt::Event& event) {
    switch (event.type()) {
    case rw::evt::EventType::key_pressed:
        on_key_pressed_event_(dynamic_cast<const rw::evt::KeyPressedEvent&>(event));
        break;
    case rw::evt::EventType::key_released:
        on_key_released_event_(dynamic_cast<const rw::evt::KeyReleasedEvent&>(event));
        break;
    case rw::evt::EventType::key_typed:
        on_key_typed_event_(dynamic_cast<const rw::evt::KeyTypedEvent&>(event));
        break;
    case rw::evt::EventType::mouse_button_pressed:
        on_mouse_button_pressed_event_(dynamic_cast<const rw::evt::MouseButtonPressedEvent&>(event));
        break;
    case rw::evt::EventType::mouse_button_released:
        on_mouse_button_released_event_(dynamic_cast<const rw::evt::MouseButtonReleasedEvent&>(event));
        break;
    case rw::evt::EventType::mouse_moved:
        on_mouse_moved_event_(dynamic_cast<const rw::evt::MouseMovedEvent&>(event));
        break;
    case rw::evt::EventType::mouse_scrolled:
        on_mouse_scrolled_event_(dynamic_cast<const rw::evt::MouseScrolledEvent&>(event));
        break;
    case rw::evt::EventType::window_resize:
        on_window_resized_event_(dynamic_cast<const rw::evt::WindowResizedEvent&>(event));
        break;
    default:
        break;
    }

    // Even if some events are handled, we return false to allow other layers to process them.
    return false;
}

ImGuiKey rw::layers::DebugLayer::imgui_key_(const rw::input::Key keycode) {
    switch (keycode) {
    case rw::input::Key::tab:
        return ImGuiKey_Tab;
    case rw::input::Key::left:
        return ImGuiKey_LeftArrow;
    case rw::input::Key::right:
        return ImGuiKey_RightArrow;
    case rw::input::Key::up:
        return ImGuiKey_UpArrow;
    case rw::input::Key::down:
        return ImGuiKey_DownArrow;
    case rw::input::Key::page_up:
        return ImGuiKey_PageUp;
    case rw::input::Key::page_down:
        return ImGuiKey_PageDown;
    case rw::input::Key::home:
        return ImGuiKey_Home;
    case rw::input::Key::end:
        return ImGuiKey_End;
    case rw::input::Key::insert:
        return ImGuiKey_Insert;
    case rw::input::Key::del:
        return ImGuiKey_Delete;
    case rw::input::Key::backspace:
        return ImGuiKey_Backspace;
    case rw::input::Key::space:
        return ImGuiKey_Space;
    case rw::input::Key::enter:
        return ImGuiKey_Enter;
    case rw::input::Key::escape:
        return ImGuiKey_Escape;
    case rw::input::Key::apostrophe:
        return ImGuiKey_Apostrophe;
    case rw::input::Key::comma:
        return ImGuiKey_Comma;
    case rw::input::Key::minus:
        return ImGuiKey_Minus;
    case rw::input::Key::period:
        return ImGuiKey_Period;
    case rw::input::Key::slash:
        return ImGuiKey_Slash;
    case rw::input::Key::semicolon:
        return ImGuiKey_Semicolon;
    case rw::input::Key::equal:
        return ImGuiKey_Equal;
    case rw::input::Key::left_bracket:
        return ImGuiKey_LeftBracket;
    case rw::input::Key::backslash:
        return ImGuiKey_Backslash;
    case rw::input::Key::world_1:
        return ImGuiKey_Oem102;
    case rw::input::Key::world_2:
        return ImGuiKey_Oem102;
    case rw::input::Key::right_bracket:
        return ImGuiKey_RightBracket;
    case rw::input::Key::grave_accent:
        return ImGuiKey_GraveAccent;
    case rw::input::Key::caps_lock:
        return ImGuiKey_CapsLock;
    case rw::input::Key::scroll_lock:
        return ImGuiKey_ScrollLock;
    case rw::input::Key::num_lock:
        return ImGuiKey_NumLock;
    case rw::input::Key::print_screen:
        return ImGuiKey_PrintScreen;
    case rw::input::Key::pause:
        return ImGuiKey_Pause;
    case rw::input::Key::keypad_0:
        return ImGuiKey_Keypad0;
    case rw::input::Key::keypad_1:
        return ImGuiKey_Keypad1;
    case rw::input::Key::keypad_2:
        return ImGuiKey_Keypad2;
    case rw::input::Key::keypad_3:
        return ImGuiKey_Keypad3;
    case rw::input::Key::keypad_4:
        return ImGuiKey_Keypad4;
    case rw::input::Key::keypad_5:
        return ImGuiKey_Keypad5;
    case rw::input::Key::keypad_6:
        return ImGuiKey_Keypad6;
    case rw::input::Key::keypad_7:
        return ImGuiKey_Keypad7;
    case rw::input::Key::keypad_8:
        return ImGuiKey_Keypad8;
    case rw::input::Key::keypad_9:
        return ImGuiKey_Keypad9;
    case rw::input::Key::keypad_decimal:
        return ImGuiKey_KeypadDecimal;
    case rw::input::Key::keypad_divide:
        return ImGuiKey_KeypadDivide;
    case rw::input::Key::keypad_multiply:
        return ImGuiKey_KeypadMultiply;
    case rw::input::Key::keypad_subtract:
        return ImGuiKey_KeypadSubtract;
    case rw::input::Key::keypad_add:
        return ImGuiKey_KeypadAdd;
    case rw::input::Key::keypad_enter:
        return ImGuiKey_KeypadEnter;
    case rw::input::Key::keypad_equal:
        return ImGuiKey_KeypadEqual;
    case rw::input::Key::left_shift:
        return ImGuiKey_LeftShift;
    case rw::input::Key::left_control:
        return ImGuiKey_LeftCtrl;
    case rw::input::Key::left_alt:
        return ImGuiKey_LeftAlt;
    case rw::input::Key::left_super:
        return ImGuiKey_LeftSuper;
    case rw::input::Key::right_shift:
        return ImGuiKey_RightShift;
    case rw::input::Key::right_control:
        return ImGuiKey_RightCtrl;
    case rw::input::Key::right_alt:
        return ImGuiKey_RightAlt;
    case rw::input::Key::right_super:
        return ImGuiKey_RightSuper;
    case rw::input::Key::menu:
        return ImGuiKey_Menu;
    case rw::input::Key::num_0:
        return ImGuiKey_0;
    case rw::input::Key::num_1:
        return ImGuiKey_1;
    case rw::input::Key::num_2:
        return ImGuiKey_2;
    case rw::input::Key::num_3:
        return ImGuiKey_3;
    case rw::input::Key::num_4:
        return ImGuiKey_4;
    case rw::input::Key::num_5:
        return ImGuiKey_5;
    case rw::input::Key::num_6:
        return ImGuiKey_6;
    case rw::input::Key::num_7:
        return ImGuiKey_7;
    case rw::input::Key::num_8:
        return ImGuiKey_8;
    case rw::input::Key::num_9:
        return ImGuiKey_9;
    case rw::input::Key::a:
        return ImGuiKey_A;
    case rw::input::Key::b:
        return ImGuiKey_B;
    case rw::input::Key::c:
        return ImGuiKey_C;
    case rw::input::Key::d:
        return ImGuiKey_D;
    case rw::input::Key::e:
        return ImGuiKey_E;
    case rw::input::Key::f:
        return ImGuiKey_F;
    case rw::input::Key::g:
        return ImGuiKey_G;
    case rw::input::Key::h:
        return ImGuiKey_H;
    case rw::input::Key::i:
        return ImGuiKey_I;
    case rw::input::Key::j:
        return ImGuiKey_J;
    case rw::input::Key::k:
        return ImGuiKey_K;
    case rw::input::Key::l:
        return ImGuiKey_L;
    case rw::input::Key::m:
        return ImGuiKey_M;
    case rw::input::Key::n:
        return ImGuiKey_N;
    case rw::input::Key::o:
        return ImGuiKey_O;
    case rw::input::Key::p:
        return ImGuiKey_P;
    case rw::input::Key::q:
        return ImGuiKey_Q;
    case rw::input::Key::r:
        return ImGuiKey_R;
    case rw::input::Key::s:
        return ImGuiKey_S;
    case rw::input::Key::t:
        return ImGuiKey_T;
    case rw::input::Key::u:
        return ImGuiKey_U;
    case rw::input::Key::v:
        return ImGuiKey_V;
    case rw::input::Key::w:
        return ImGuiKey_W;
    case rw::input::Key::x:
        return ImGuiKey_X;
    case rw::input::Key::y:
        return ImGuiKey_Y;
    case rw::input::Key::z:
        return ImGuiKey_Z;
    case rw::input::Key::f1:
        return ImGuiKey_F1;
    case rw::input::Key::f2:
        return ImGuiKey_F2;
    case rw::input::Key::f3:
        return ImGuiKey_F3;
    case rw::input::Key::f4:
        return ImGuiKey_F4;
    case rw::input::Key::f5:
        return ImGuiKey_F5;
    case rw::input::Key::f6:
        return ImGuiKey_F6;
    case rw::input::Key::f7:
        return ImGuiKey_F7;
    case rw::input::Key::f8:
        return ImGuiKey_F8;
    case rw::input::Key::f9:
        return ImGuiKey_F9;
    case rw::input::Key::f10:
        return ImGuiKey_F10;
    case rw::input::Key::f11:
        return ImGuiKey_F11;
    case rw::input::Key::f12:
        return ImGuiKey_F12;
    case rw::input::Key::f13:
        return ImGuiKey_F13;
    case rw::input::Key::f14:
        return ImGuiKey_F14;
    case rw::input::Key::f15:
        return ImGuiKey_F15;
    case rw::input::Key::f16:
        return ImGuiKey_F16;
    case rw::input::Key::f17:
        return ImGuiKey_F17;
    case rw::input::Key::f18:
        return ImGuiKey_F18;
    case rw::input::Key::f19:
        return ImGuiKey_F19;
    case rw::input::Key::f20:
        return ImGuiKey_F20;
    case rw::input::Key::f21:
        return ImGuiKey_F21;
    case rw::input::Key::f22:
        return ImGuiKey_F22;
    case rw::input::Key::f23:
        return ImGuiKey_F23;
    case rw::input::Key::f24:
        return ImGuiKey_F24;
    default:
        return ImGuiKey_None;
    }
}

void rw::layers::DebugLayer::on_key_pressed_event_(const rw::evt::KeyPressedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.KeysData[static_cast<int>(imgui_key_(event.key)) - ImGuiKey_NamedKey_BEGIN].Down = true;
    io.AddKeyEvent(imgui_key_(event.key), true);
    io.SetKeyEventNativeData(imgui_key_(event.key), static_cast<int>(event.key), 0);
    update_control_keys_();
}

void rw::layers::DebugLayer::on_key_released_event_(const rw::evt::KeyReleasedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.KeysData[static_cast<int>(imgui_key_(event.key)) - ImGuiKey_NamedKey_BEGIN].Down = false;
    io.AddKeyEvent(imgui_key_(event.key), false);
    io.SetKeyEventNativeData(imgui_key_(event.key), static_cast<int>(event.key), 0);
    update_control_keys_();
}

void rw::layers::DebugLayer::on_key_typed_event_(const rw::evt::KeyTypedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.AddInputCharacter(static_cast<unsigned int>(event.key));
}

void rw::layers::DebugLayer::on_mouse_button_pressed_event_(const rw::evt::MouseButtonPressedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.MouseDown[static_cast<size_t>(event.button)] = true;
    update_control_keys_();
}

void rw::layers::DebugLayer::on_mouse_button_released_event_(const rw::evt::MouseButtonReleasedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.MouseDown[static_cast<size_t>(event.button)] = false;
    update_control_keys_();
}

void rw::layers::DebugLayer::on_mouse_moved_event_(const rw::evt::MouseMovedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.MousePos = ImVec2(static_cast<float>(event.x), static_cast<float>(event.y));
}

void rw::layers::DebugLayer::on_mouse_scrolled_event_(const rw::evt::MouseScrolledEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.MouseWheelH += static_cast<float>(event.x_offset);
    io.MouseWheel += static_cast<float>(event.y_offset);
}

void rw::layers::DebugLayer::on_window_resized_event_(const rw::evt::WindowResizedEvent& event) {
    ImGuiIO& io{ ImGui::GetIO() };
    io.DisplaySize             = ImVec2(static_cast<float>(event.width), static_cast<float>(event.height));
    io.DisplayFramebufferScale = ImVec2(1.0F, 1.0F);
    glViewport(0, 0, static_cast<int>(event.width), static_cast<int>(event.height));
}

void rw::layers::DebugLayer::update_control_keys_() {
    ImGuiIO& io{ ImGui::GetIO() };
    io.AddKeyEvent(ImGuiMod_Ctrl, rw::input::is_control_down());
    io.AddKeyEvent(ImGuiMod_Shift, rw::input::is_shift_down());
    io.AddKeyEvent(ImGuiMod_Alt, rw::input::is_alt_down());
    io.AddKeyEvent(ImGuiMod_Super, input::is_super_down());
}
