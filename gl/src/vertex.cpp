#include "vertex.hpp"

namespace gl {
  VertexAttributeElement::VertexAttributeElement(unsigned int type,
                                                 unsigned int count,
                                                 unsigned int normalized) {
    this->type = type;
    this->count = count;
    this->normalized = normalized;
  }

  unsigned int VertexAttributeElement::getType() const {
    return this->type;
  }

  unsigned int VertexAttributeElement::getCount() const {
    return this->count;
  }

  unsigned int VertexAttributeElement::isNormalised() const {
    return this->normalized;
  }

  const unsigned int VertexAttributeElement::getSizeOfElement() const {
    if (this->type == GL_FLOAT) {
      return sizeof(GLfloat);
    } else if (this->type == GL_INT) {
      return sizeof(GLint);
    } else {
      return 0;
    }
  }
}

namespace gl {
  const std::vector<VertexAttributeElement>
      VertexAttribute::getElements() const {
    return this->elements;
  }

  const unsigned int VertexAttribute::getStride() const {
    return this->stride;
  }

  template <> inline void VertexAttribute::push<float>(unsigned int count) {
    this->elements.push_back(VertexAttributeElement(GL_FLOAT, count, GL_FALSE));
    this->stride += count * this->elements.back().getSizeOfElement();
  }

  template <> inline void VertexAttribute::push<int>(unsigned int count) {
    this->elements.push_back(VertexAttributeElement(GL_INT, count, GL_FALSE));
    this->stride += count * this->elements.back().getSizeOfElement();
  }

  template <> inline void VertexAttribute::push<glm::vec3>(unsigned int count) {
    for (int i = 0; i < count; i++)
      this->push<float>(3);
  }

  template <> inline void VertexAttribute::push<glm::vec2>(unsigned int count) {
    for (int i = 0; i < count; i++)
      this->push<float>(2);
  }
}

namespace gl {
  Vertex::Vertex(glm::vec3 position, glm::vec3 color,
                 glm::vec2 textureCoordinate) {
    this->position = position;
    this->color = color;
    this->textureCoordinate = textureCoordinate;
  }

  void Vertex::setPosition(glm::vec3 position) {
    this->position = position;
  }

  void Vertex::setColor(glm::vec3 color) {
    this->color = color;
  }

  void Vertex::setTextureCoordinate(glm::vec2 textureCoordinate) {
    this->textureCoordinate = textureCoordinate;
  }

  glm::vec3 Vertex::getPosition() const {
    return this->position;
  }

  glm::vec3 Vertex::getColor() const {
    return this->color;
  }

  glm::vec2 Vertex::getTextureCoordinate() const {
    return this->textureCoordinate;
  }

  VertexAttribute Vertex::getAttribute() {
    VertexAttribute vertexAttribute;
    vertexAttribute.push<glm::vec3>(2);
    vertexAttribute.push<glm::vec2>(1);
    return vertexAttribute;
  }
}