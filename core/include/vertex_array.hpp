#pragma once

#include "vertex.hpp"

namespace core {
  class VertexArray {
  private:
    unsigned int vertexArrayIndex;
    unsigned int vertexBufferIndex;
    unsigned int elementBufferIndex;

    void generateAndAttachBuffer(unsigned int &index, int target);
    void arrangeVertexLayout();

  public:
    VertexArray(std::vector<Vertex>       &vertices,
                std::vector<unsigned int> &indices);
    ~VertexArray();

    void bind();
  };
}