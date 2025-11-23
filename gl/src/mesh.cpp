#include "mesh.hpp"
#include "gl_utils.hpp"

namespace gl {
  Mesh::Mesh(std::vector<core::Vertex> &vertices,
             std::vector<unsigned int> &indices)
      : vertexArray(vertices, indices), vertexCount(vertices.size()),
        indexCount(indices.size()) {}

  void Mesh::clearComponent() {
    this->vertexArray.releaseBuffers();
  }

  void Mesh::draw() {
    this->vertexArray.bind();
    if (indexCount > 0) {
      GL_CALL(
          glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0))
    } else {
      GL_CALL(glDrawArrays(GL_TRIANGLES, 0, this->vertexCount));
    }
    this->vertexArray.unbind();
  }
}