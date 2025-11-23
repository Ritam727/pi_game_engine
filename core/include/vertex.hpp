#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace core {
  class VertexAttributeElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

  public:
    VertexAttributeElement(unsigned int type = 0, unsigned int count = 0,
                           unsigned int normalized = 0);

    unsigned int getType() const;
    unsigned int getCount() const;
    unsigned int isNormalised() const;

    const unsigned int getSizeOfElement() const;
  };

  struct VertexAttribute {
    unsigned int                        stride = 0;
    std::vector<VertexAttributeElement> elements;

    template <typename T> inline void push(unsigned int count);
  };

  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 textureCoordinates{};

    Vertex();

    Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal,
           glm::vec2 textureCoordinates);

    static VertexAttribute getAttribute();
  };
}