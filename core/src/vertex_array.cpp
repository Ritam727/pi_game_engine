#include "vertex_array.hpp"
#include "vertex.hpp"

namespace core {
  VertexArray::VertexArray(std::vector<Vertex>       &vertices,
                           std::vector<unsigned int> &indices) {
    glGenVertexArrays(1, &this->vertexArrayIndex);
    glBindVertexArray(this->vertexArrayIndex);
    this->generateAndAttachBuffer(this->vertexBufferIndex, GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);
    this->generateAndAttachBuffer(this->elementBufferIndex,
                                  GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    this->arrangeVertexLayout();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void VertexArray::generateAndAttachBuffer(unsigned int &index, int target) {
    glGenBuffers(1, &index);
    glBindBuffer(target, index);
  }

  void VertexArray::arrangeVertexLayout() {
    unsigned int    offset = 0;
    VertexAttribute vertexAttribute = Vertex::getAttribute();
    const std::vector<VertexAttributeElement> vertexAttributeElements =
        vertexAttribute.getElements();
    for (int i = 0; i < vertexAttributeElements.size(); i++) {
      glVertexAttribPointer(i, vertexAttributeElements[i].getCount(),
                            vertexAttributeElements[i].getType(),
                            vertexAttributeElements[i].isNormalised(),
                            vertexAttribute.getStride(),
                            (const void *) ((std::size_t) offset));
      glEnableVertexAttribArray(i);
      offset += vertexAttributeElements[i].getCount() *
                vertexAttributeElements[i].getSizeOfElement();
    }
  }

  VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &this->vertexArrayIndex);
    glDeleteBuffers(1, &this->vertexBufferIndex);
    glDeleteBuffers(1, &this->elementBufferIndex);
  }

  void VertexArray::bind() {
    glBindVertexArray(this->vertexArrayIndex);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferIndex);
  }
}