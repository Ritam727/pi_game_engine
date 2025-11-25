#include "ui.hpp"

#include "camera_transform.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "selectable.hpp"
#include "transform.hpp"

#include <string>

namespace ui {
  UI::UI(core::Window &window, core::Registry &registry)
      : window(window), registry(registry) {
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

    this->dockspace();

    this->entities();
    this->components();
  }

  void UI::postUpdate() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void UI::dockspace() {
    ImGui::NewFrame();
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags   windowFlags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }
    ImGui::End();
  }

  void UI::entities() {
    ImGui::Begin("Entities");
    std::vector<core::Entity> &selectableEntities =
        this->registry.getPool<core::Selectable>().getEntities();
    for (unsigned int i = 0;
         i < this->registry.getPool<core::Selectable>().getNumElements(); i++) {
      core::Entity &entity = selectableEntities[i];
      this->registry.addComponent<core::Selectable>(entity, false);
      std::string str = "Entity " + std::to_string(entity);
      ImGui::Checkbox(
          str.c_str(),
          &this->registry.getPool<core::Selectable>().get(entity).selected);
      if (this->registry.getPool<core::Selectable>().get(entity).selected) {
        this->selectedEntities.addElem(entity);
      } else {
        this->selectedEntities.removeElem(entity);
      }
    }
    ImGui::End();
  }

  void UI::components() {
    ImGui::Begin("Components");
    std::vector<core::Entity> &entities = this->selectedEntities.getEntities();
    for (unsigned int i = 0; i < this->selectedEntities.getNumElements(); i++) {
      core::Entity &entity = entities[i];
      std::string   str;
      if (this->registry.getPool<core::Transform>().contains(entity))
        str = "Transform " + std::to_string(entity);
      if (this->registry.getPool<core::CameraTransform>().contains(entity))
        str = "Camera Transform " + std::to_string(entity);
      ImGui::Text("%s", str.c_str());
    }
    ImGui::End();
  }

  UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
}