#pragma once

#include "camera.hpp"
#include "inputs.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "resource_manager.hpp"
#include "window.hpp"
#include "constants.hpp"
#include "inputs_constants.hpp"
#include "app_config.hpp"

struct ScreenSize {
  int width;
  int height;
};

class App {
private:
  core::ResourceManager resourceManager{};
  core::EventManager    eventManager{};
  core::Registry        registry{};
  core::Window   window{App::getScreenSize().width, App::getScreenSize().height,
                      AppConfig::NAME, this->eventManager};
  core::Camera   camera{this->registry, glm::vec3(0.0f, 0.0f, 3.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f)};
  inputs::Inputs inputs{this->window, this->registry, this->eventManager};
  std::vector<std::unique_ptr<core::Layer>> layers{};
  bool                                      running{true};

  static const inline std::vector<std::string> nonMainThreadTopics{
      inputs::Constants::FOV_CHANGE_TOPIC,
      inputs::Constants::MOUSE_VISIBLE_TOPIC};
  static const inline std::vector<std::string> mainThreadTopics{
      core::Constants::WINDOW_CLOSE_TOPIC,
      core::Constants::WINDOW_RESIZE_TOPIC,
      core::Constants::KEY_STATE_TOPIC,
      core::Constants::MOUSE_BUTTON_TOPIC,
      core::Constants::MOUSE_MOVEMENT_TOPIC,
      core::Constants::MOUSE_SCROLL_TOPIC};

public:
  App();

  void run();

  static ScreenSize &getScreenSize();
};