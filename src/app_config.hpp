#pragma once

#include <string>

class AppConfig {
private:
  int         width;
  int         height;
  std::string name;

public:
  AppConfig() : width(800), height(600), name("Learn OpenGL") {}

  inline void setWidth(int width) {
    this->width = width;
  }

  inline void setHeight(int height) {
    this->height = height;
  }

  inline const std::string &getName() const {
    return this->name;
  }

  inline int &getWidth() {
    return this->width;
  }

  inline int &getHeight() {
    return this->height;
  }
};