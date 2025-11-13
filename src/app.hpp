#pragma once

#include "camera.hpp"
#include "events.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "window.hpp"

#include "app_config.hpp"

struct ScreenSize {
  int width;
  int height;
};

class App {
private:
  core::Window   window;
  core::Registry registry;

  core::Camera camera;

  std::vector<std::unique_ptr<core::Layer>> layers;

public:
  App();

  void run();

  static void windowCloseHandler(core::InputEvent &event);
  static void mouseButtonHandler(core::InputEvent &event);
  static void windowResizeHandler(core::InputEvent &event);

  static bool       &isRunning();
  static ScreenSize &getScreenSize();
};