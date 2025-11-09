#pragma once

#include "glad/glad.h"

#include <string>

namespace gl {
  class Shader {
  private:
    unsigned int program;

    static unsigned int createShader(char const *sourceCode, GLenum type);
    void                createAndLinkProgram(unsigned int vertexShader,
                                             unsigned int fragmentShader);

  public:
    Shader(char const *vertexFilePath, char const *fragmentFilePath);
    ~Shader();

    void use();

    template <typename T> void set(const std::string &name, T value) const;
  };
}