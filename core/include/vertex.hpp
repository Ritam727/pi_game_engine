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

  class VertexAttribute {
  private:
    unsigned int                        stride = 0;
    std::vector<VertexAttributeElement> elements;

  public:
    const std::vector<VertexAttributeElement> getElements() const;
    const unsigned int                        getStride() const;

    template <typename T> inline void push(unsigned int count) {}
  };

  class Vertex {
  private:
    glm::vec3 position;
    // glm::vec3 color;
    glm::vec2 textureCoordinate;

  public:
    Vertex(glm::vec3 position, glm::vec2 textureCoordinate);

    void setPosition(glm::vec3 position);
    void setColor(glm::vec3 color);
    void setTextureCoordinate(glm::vec2 textureCoordinate);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    glm::vec2 getTextureCoordinate() const;

    static VertexAttribute getAttribute();
  };
}