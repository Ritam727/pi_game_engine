#pragma once

#include "renderer.hpp"
#include "window.hpp"

#include "app_config.hpp"

class App {
private:
  core::Window window;
  gl::Renderer renderer;

  static AppConfig config;
  static bool      running;

public:
  App();

  void run();

  static void windowCloseHandler(core::InputEvent &event);
  static void windowResizeHandler(core::InputEvent &event);
  static void mouseMovementHandler(core::InputEvent &event);
  static void mouseButtonHandler(core::InputEvent &event);
  static void mouseScrollHandler(core::InputEvent &event);
};