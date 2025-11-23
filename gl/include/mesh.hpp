#pragma once

#include "base_component.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"

namespace gl {
  class Mesh : core::BaseComponent {
  private:
    VertexArray  vertexArray;
    unsigned int vertexCount;
    unsigned int indexCount;

  public:
    Mesh(std::vector<core::Vertex> &vertices,
         std::vector<unsigned int> &indices);

    void clearComponent() override;
    void draw();
  };
};