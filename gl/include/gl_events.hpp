#pragma once

#include <string>
namespace gl {
  struct FovChangeEvent {
    float fov;

    FovChangeEvent(float fov) {
      this->fov = fov;
    }

    bool operator==(const FovChangeEvent &event) const {
      return this->fov == event.fov;
    }
  };

  struct ModelLoadEvent {
    std::string path;

    ModelLoadEvent(std::string path) : path(path) {}

    bool operator==(const ModelLoadEvent &event) const {
      return this->path == event.path;
    }
  };
}