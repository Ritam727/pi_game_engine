#include "shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils.hpp"

namespace gl {
  Shader::Shader(char const *vertexFilePath, char const *fragmentFilePath) {
    std::string vertexShaderCode = gl::readFromFile(vertexFilePath);
    std::string fragmentShaderCode = gl::readFromFile(fragmentFilePath);

    unsigned int vertexShader =
        createShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader =
        createShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

    this->createAndLinkProgram(vertexShader, fragmentShader);

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));
  }

  Shader::~Shader() {
    GL_CALL(glDeleteProgram(this->program));
  }

  unsigned int Shader::createShader(char const *sourceCode, GLenum type) {
    unsigned int shader;
    GL_CALL_WITH_ASSIGNMENT(glCreateShader(type), shader);
    GL_CALL(glShaderSource(shader, 1, &sourceCode, nullptr));
    GL_CALL(glCompileShader(shader));
    CHECK_GL_STATUS(shader, glGetShaderiv, GL_COMPILE_STATUS,
                    glGetShaderInfoLog);
    return shader;
  }

  void Shader::createAndLinkProgram(unsigned int vertexShader,
                                    unsigned int fragmentShader) {
    GL_CALL_WITH_ASSIGNMENT(glCreateProgram(), this->program);
    GL_CALL(glAttachShader(this->program, vertexShader));
    GL_CALL(glAttachShader(this->program, fragmentShader));
    GL_CALL(glLinkProgram(this->program));
    CHECK_GL_STATUS(this->program, glGetProgramiv, GL_LINK_STATUS,
                    glGetProgramInfoLog);
  }

  void Shader::use() {
    GL_CALL(glUseProgram(this->program));
  }

  template <> void Shader::set<int>(const std::string &name, int value) const {
    GL_CALL(
        glUniform1i(glGetUniformLocation(this->program, name.c_str()), value));
  }

  template <>
  void Shader::set<float>(const std::string &name, float value) const {
    GL_CALL(
        glUniform1f(glGetUniformLocation(this->program, name.c_str()), value));
  }

  template <>
  void Shader::set<glm::vec2>(const std::string &name, glm::vec2 value) const {
    GL_CALL(glUniform2f(glGetUniformLocation(this->program, name.c_str()),
                        value.x, value.y));
  }

  template <>
  void Shader::set<glm::vec3>(const std::string &name, glm::vec3 value) const {
    GL_CALL(glUniform3f(glGetUniformLocation(this->program, name.c_str()),
                        value.x, value.y, value.z));
  }

  template <>
  void Shader::set<glm::vec4>(const std::string &name, glm::vec4 value) const {
    GL_CALL(glUniform4f(glGetUniformLocation(this->program, name.c_str()),
                        value.x, value.y, value.z, value.w));
  }

  template <>
  void Shader::set<glm::mat4>(const std::string &name, glm::mat4 value) const {
    GL_CALL(
        glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1,
                           GL_FALSE, glm::value_ptr(value)));
  }
}