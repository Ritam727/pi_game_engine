#include "ui.hpp"

#include "entities.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "components.hpp"

namespace ui {
  UI::UI(core::Window &window, core::Registry &registry)
      : window(window), registry(registry) {
    this->uiLayers.pushLayer<Components>(this->uiState);
    this->uiLayers.pushLayer<Entities>(this->uiState);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init();
  }

  void UI::onUpdate(float ts) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, nullptr,
                                 ImGuiDockNodeFlags_PassthruCentralNode);
    this->uiLayers.onUpdate(ts, this->registry);
  }

  void UI::postUpdate() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
}