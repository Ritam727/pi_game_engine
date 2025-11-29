#include "ui.hpp"

#include "entities.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "components.hpp"
#include "ui_constants.hpp"

namespace ui {
  UI::UI(core::Window &window, core::Registry &registry)
      : window(window), registry(registry) {
    this->uiLayers.pushLayer<Components>(this->uiState);
    this->uiLayers.pushLayer<Entities>();

    IMGUI_CHECKVERSION();
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

    UI::createDockspace();

    this->uiLayers.onUpdate(ts, this->selectedEntities, this->registry);
  }

  void UI::postUpdate() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void UI::createDockspace() {
    ImGui::NewFrame();
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags   windowFlags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Constants::ZERO);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, Constants::ZERO);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                        ImVec2(Constants::ZERO, Constants::ZERO));
    ImGui::Begin(Constants::DOCKSPACE.c_str(), nullptr, windowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspaceId = ImGui::GetID(Constants::DOCKSPACE.c_str());
      ImGui::DockSpace(dockspaceId, ImVec2(Constants::ZERO, Constants::ZERO),
                       dockspaceFlags);
    }
    ImGui::End();
  }

  UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
}