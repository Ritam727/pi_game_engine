#include "shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils.hpp"

namespace core {
  Shader::Shader(char const *vertexFilePath, char const *fragmentFilePath) {
    std::string vertexShaderCode = core::readFromFile(vertexFilePath);
    std::string fragmentShaderCode = core::readFromFile(fragmentFilePath);

    unsigned int vertexShader =
        createShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader =
        createShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

    this->createAndLinkProgram(vertexShader, fragmentShader);

    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);
  }

  Shader::~Shader() {
    glDeleteProgram(this->program);
  }

  unsigned int Shader::createShader(char const *sourceCode, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceCode, nullptr);
    glCompileShader(shader);
    CHECK_GL_STATUS(shader, glGetShaderiv, GL_COMPILE_STATUS,
                    glGetShaderInfoLog);
    return shader;
  }

  void Shader::createAndLinkProgram(unsigned int vertexShader,
                                    unsigned int fragmentShader) {
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);
    CHECK_GL_STATUS(this->program, glGetProgramiv, GL_LINK_STATUS,
                    glGetProgramInfoLog);
  }

  void Shader::use() {
    glUseProgram(this->program);
  }

  template <> void Shader::set<int>(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), value);
  }

  template <>
  void Shader::set<float>(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), value);
  }

  template <>
  void Shader::set<glm::vec2>(const std::string &name, glm::vec2 value) const {
    glUniform2f(glGetUniformLocation(this->program, name.c_str()), value.x,
                value.y);
  }

  template <>
  void Shader::set<glm::vec3>(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(this->program, name.c_str()), value.x,
                value.y, value.z);
  }

  template <>
  void Shader::set<glm::vec4>(const std::string &name, glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(this->program, name.c_str()), value.x,
                value.y, value.x, value.w);
  }

  template <>
  void Shader::set<glm::mat4>(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1,
                       GL_FALSE, glm::value_ptr(value));
  }
}