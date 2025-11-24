#pragma once

namespace core {
  class Layer {
  public:
    virtual void onAttach() {}
    virtual void onDetach() {}

    virtual void onUpdate(float ts) = 0;

    virtual ~Layer() = default;
  };
}