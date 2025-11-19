#pragma once

#include "camera.hpp"
#include "events.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "window.hpp"
#include "state_manager.hpp"
#include "constants.hpp"
#include "inputs_constants.hpp"

struct ScreenSize {
  int width;
  int height;
};

class App {
private:
  core::Window                              window;
  core::Registry                            registry;
  core::Camera                              camera;
  std::vector<std::unique_ptr<core::Layer>> layers;

  static const inline std::vector<std::string> nonMainThreadTopics{
      inputs::Constants::FOV_CHANGE_TOPIC};
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

  static void windowCloseHandler(std::unique_ptr<core::BaseEvent> &event);
  static void eventManagerThread();

  static bool       &isRunning();
  static ScreenSize &getScreenSize();
};