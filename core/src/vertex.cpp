#include "vertex.hpp"

namespace core {
  VertexAttributeElement::VertexAttributeElement(unsigned int type,
                                                 unsigned int count,
                                                 unsigned int normalized)
      : type(type), count(count), normalized(normalized) {}

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

namespace core {
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

namespace core {
  Vertex::Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal,
                 glm::vec2 textureCoordinates)
      : position(position), color(color), normal(normal),
        textureCoordinates(textureCoordinates) {}

  VertexAttribute Vertex::getAttribute() {
    VertexAttribute vertexAttribute;
    vertexAttribute.push<glm::vec3>(3);
    vertexAttribute.push<glm::vec2>(1);
    return vertexAttribute;
  }
}