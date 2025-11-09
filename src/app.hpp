#pragma once

#include "renderer.hpp"
#include "window.hpp"

#include "app_config.hpp"

class App {
private:
  gl::Window   window;
  gl::Renderer renderer;

  static AppConfig config;
  static bool      running;

public:
  App();

  void run();

  static void windowCloseCallback(core::BasicEvent &event);
  static void windowResizeCallback(core::BasicEvent &event);
  static void eventThreadExecutor();
};