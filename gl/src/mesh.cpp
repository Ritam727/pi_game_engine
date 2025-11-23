#include "mesh.hpp"
#include "gl_utils.hpp"

namespace gl {
  Mesh::Mesh(std::vector<core::Vertex> &vertices,
             std::vector<unsigned int> &indices)
      : vertexArray(vertices, indices), count(vertices.size()),
        indexed(!indices.empty()) {}

  void Mesh::clearComponent() {
    this->vertexArray.releaseBuffers();
  }

  void Mesh::draw() {
    this->vertexArray.bind();
    if (indexed)
      GL_CALL(glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0))
    else
      GL_CALL(glDrawArrays(GL_TRIANGLES, 0, this->count));
    this->vertexArray.unbind();
  }
}