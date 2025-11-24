#pragma once

#include "camera.hpp"
#include "inputs.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "resource_manager.hpp"
#include "window.hpp"
#include "gl_constants.hpp"
#include "constants.hpp"
#include "inputs_constants.hpp"
#include "app_config.hpp"

struct ScreenSize {
  int width;
  int height;
};

class App {
private:
  ScreenSize            screenSize{1920, 1080};
  core::ResourceManager resourceManager{};
  core::EventManager    eventManager{};
  core::Registry        registry{};
  core::Window   window{screenSize.width, screenSize.height, AppConfig::NAME,
                      this->eventManager};
  core::Camera   camera{this->registry, glm::vec3(0.0f, 0.0f, 10.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f)};
  inputs::Inputs inputs{this->window, this->registry, this->eventManager};
  std::vector<std::unique_ptr<core::Layer>> layers{};
  bool                                      running{true};

  static const inline std::vector<std::string> nonMainThreadTopics{
      inputs::Constants::FOV_CHANGE_TOPIC,
      inputs::Constants::MOUSE_VISIBLE_TOPIC};
  static const inline std::vector<std::string> mainThreadTopics{
      gl::Constants::MODEL_LOAD_TOPIC,
      core::Constants::WINDOW_RESIZE_TOPIC,
      core::Constants::KEY_STATE_TOPIC,
      core::Constants::MOUSE_BUTTON_TOPIC,
      core::Constants::MOUSE_MOVEMENT_TOPIC,
      core::Constants::MOUSE_SCROLL_TOPIC,
      core::Constants::WINDOW_CLOSE_TOPIC};

public:
  App();

  void run();
};