#include "ui.hpp"

#include "camera_transform.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "lights.hpp"
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

    this->createDockspace();

    this->entitiesWindow();
    this->componentsWindow();
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

  void UI::entitiesWindow() {
    ImGui::Begin("Entities");
    std::vector<core::Entity> &selectableEntities =
        this->registry.getPool<core::Selectable>().getEntities();

    for (unsigned int i = 0;
         i < this->registry.getPool<core::Selectable>().getNumElements(); i++) {
      core::Entity &entity = selectableEntities[i];
      std::string   str = "Entity " + std::to_string(entity);

      bool &isEntitySelected =
          this->registry.getPool<core::Selectable>().get(entity).selected;

      ImGui::Checkbox(str.c_str(), &isEntitySelected);

      if (isEntitySelected) {
        this->selectedEntities.addElem(entity);
      } else {
        this->selectedEntities.removeElem(entity);
      }
    }
    ImGui::End();
  }

  void UI::componentsWindow() {
    ImGui::Begin("Components");
    std::vector<core::Entity> &entities = this->selectedEntities.getEntities();
    for (unsigned int i = 0; i < this->selectedEntities.getNumElements(); i++) {
      core::Entity &entity = entities[i];
      std::string   str;
      if (this->registry.getPool<core::Transform>().contains(entity)) {
        str = "Transform " + std::to_string(entity);
        ImGui::Text("%s", str.c_str());
      }
      if (this->registry.getPool<core::CameraTransform>().contains(entity)) {
        str = "Camera Transform " + std::to_string(entity);
        ImGui::Text("%s", str.c_str());
      }
      if (this->registry.getPool<gl::LightComponent>().contains(entity)) {
        this->lightComponent(
            entity, this->registry.getPool<gl::LightComponent>().get(entity));
      }
    }
    ImGui::End();
  }

  void UI::lightComponent(core::Entity &entity, gl::LightComponent &light) {
    ImGui::Text("Light Controls");
    std::vector<std::string> items{"Directional", "Point", "Spot"};
    this->dropDownMenu<gl::LightType>(items, "Type", light.type);
    this->colorControls(entity, light.ambient, light.diffuse, light.specular);
    switch (light.type) {
    case gl::LightType::DIRECTIONAL_LIGHT:
      this->directionalLightControls(entity, light);
      break;
    case gl::LightType::POINT_LIGHT:
      this->pointLightControls(entity, light);
      break;
    case gl::LightType::SPOT_LIGHT:
      this->spotLightControls(entity, light);
      break;
    default:
      break;
    }
  }

  void UI::directionalLightControls(core::Entity       &entity,
                                    gl::LightComponent &light) {
    this->dragFloat3("Light" + std::to_string(entity), "Direction",
                     light.direction);
  }

  void UI::pointLightControls(core::Entity &entity, gl::LightComponent &light) {
    this->dragFloat3("Light" + std::to_string(entity), "Position",
                     light.position);
    this->dragFloat("Light" + std::to_string(entity), "Linear", light.linear,
                    0.001);
    this->dragFloat("Light" + std::to_string(entity), "Quadratic",
                    light.quadratic, 0.001);
  }

  void UI::spotLightControls(core::Entity &entity, gl::LightComponent &light) {
    this->dragFloat3("Light" + std::to_string(entity), "Direction",
                     light.direction);
    this->dragFloat3("Light" + std::to_string(entity), "Position",
                     light.position);
    this->dragFloat("Light" + std::to_string(entity), "Linear", light.linear,
                    0.001);
    this->dragFloat("Light" + std::to_string(entity), "Quadratic",
                    light.quadratic, 0.001);
    this->dragFloat("Light" + std::to_string(entity), "Inner Cut Off",
                    light.innerCutOff);
    this->dragFloat("Light" + std::to_string(entity), "Outer Cut Off",
                    light.outerCutOff);
    light.outerCutOff = std::max(light.outerCutOff, light.innerCutOff + 0.1f);
  }

  void UI::colorControls(core::Entity &entity, glm::vec3 &ambient,
                         glm::vec3 &diffuse, glm::vec3 &specular) {
    this->dragFloat3("Light" + std::to_string(entity), "Ambient", ambient);
    this->dragFloat3("Light" + std::to_string(entity), "Diffuse", diffuse);
    this->dragFloat3("Light" + std::to_string(entity), "Specular", specular);
  }

  void UI::dragFloat3(const std::string &ID, const std::string &label,
                      glm::vec3 &value, float delta) {
    ImGui::PushID((ID + label).c_str());
    ImGui::DragFloat3(label.c_str(), &value.x, delta);
    ImGui::PopID();
  }

  void UI::dragFloat(const std::string &ID, const std::string &label,
                     float &value, float delta) {
    ImGui::PushID((ID + label).c_str());
    ImGui::DragFloat(label.c_str(), &value, delta);
    ImGui::PopID();
  }

  UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
}