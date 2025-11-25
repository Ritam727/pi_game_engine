#include "vertex_array.hpp"
#include "gl_utils.hpp"
#include "vertex.hpp"

namespace gl {
  VertexArray::VertexArray() {}

  VertexArray::VertexArray(std::vector<core::Vertex> &vertices,
                           std::vector<unsigned int> &indices) {
    GL_CALL(glGenVertexArrays(1, &this->vertexArrayIndex));
    this->bind();
    this->generateAndAttachBuffer(this->vertexBufferIndex, GL_ARRAY_BUFFER);
    GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(core::Vertex) * vertices.size(),
                         vertices.data(), GL_STATIC_DRAW));
    this->generateAndAttachBuffer(this->elementBufferIndex,
                                  GL_ELEMENT_ARRAY_BUFFER);
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(unsigned int) * indices.size(), indices.data(),
                         GL_STATIC_DRAW));
    this->arrangeVertexLayout();
    this->unbind();
  }

  void VertexArray::generateAndAttachBuffer(unsigned int &index, int target) {
    GL_CALL(glGenBuffers(1, &index));
    GL_CALL(glBindBuffer(target, index));
  }

  void VertexArray::arrangeVertexLayout() {
    unsigned int          offset = 0;
    core::VertexAttribute vertexAttribute = core::Vertex::getAttribute();
    const std::vector<core::VertexAttributeElement> &vertexAttributeElements =
        vertexAttribute.elements;
    for (int i = 0; i < vertexAttributeElements.size(); i++) {
      GL_CALL(glVertexAttribPointer(i, vertexAttributeElements[i].getCount(),
                                    vertexAttributeElements[i].getType(),
                                    vertexAttributeElements[i].isNormalised(),
                                    vertexAttribute.stride,
                                    (const void *) ((std::size_t) offset)));
      GL_CALL(glEnableVertexAttribArray(i));
      offset += vertexAttributeElements[i].getCount() *
                vertexAttributeElements[i].getSizeOfElement();
    }
  }

  void VertexArray::bind() {
    GL_CALL(glBindVertexArray(this->vertexArrayIndex));
  }

  void VertexArray::unbind() {
    GL_CALL(glBindVertexArray(0));
  }

  void VertexArray::releaseBuffers() {
    GL_CALL(glDeleteVertexArrays(1, &this->vertexArrayIndex));
    GL_CALL(glDeleteBuffers(1, &this->vertexBufferIndex));
    GL_CALL(glDeleteBuffers(1, &this->elementBufferIndex));
  }
}