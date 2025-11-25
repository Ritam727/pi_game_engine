#pragma once

#include "vertex.hpp"

namespace gl {
  class VertexArray {
  private:
    unsigned int vertexArrayIndex;
    unsigned int vertexBufferIndex;
    unsigned int elementBufferIndex;

    void generateAndAttachBuffer(unsigned int &index, int target);
    void arrangeVertexLayout();

  public:
    VertexArray();
    VertexArray(std::vector<core::Vertex> &vertices,
                std::vector<unsigned int> &indices);

    void bind();
    void unbind();
    void releaseBuffers();
  };
}