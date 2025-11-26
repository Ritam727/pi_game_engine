#include "shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gl_utils.hpp"
#include "lights.hpp"
#include "materials.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"
#include "utils.hpp"

namespace gl {
  Shader::Shader(char const *vertexFilePath, char const *fragmentFilePath) {
    std::string vertexShaderCode = core::readFromFile(vertexFilePath);
    std::string fragmentShaderCode = core::readFromFile(fragmentFilePath);

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

  template <>
  void Shader::set<bool>(const std::string &name, bool value) const {
    GL_CALL(
        glUniform1i(glGetUniformLocation(this->program, name.c_str()), value));
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

  template <>
  void Shader::set<LightComponent &>(const std::string &name,
                                     LightComponent    &light) const {
    this->set<int>(name + ".type", static_cast<int>(light.type));
    this->set<glm::vec3>(name + ".ambient", light.ambient);
    this->set<glm::vec3>(name + ".diffuse", light.diffuse);
    this->set<glm::vec3>(name + ".specular", light.specular);

    switch (light.type) {
    case LightType::DIRECTIONAL_LIGHT:
      this->set<glm::vec3>(name + ".direction", light.direction);
      break;
    case LightType::POINT_LIGHT:
      this->set<glm::vec3>(name + ".position", light.position);
      this->set<float>(name + ".constant", light.constant);
      this->set<float>(name + ".linear", light.linear);
      this->set<float>(name + ".quadratic", light.quadratic);
      break;
    case LightType::SPOT_LIGHT:
      this->set<glm::vec3>(name + ".position", light.position);
      this->set<glm::vec3>(name + ".direction", light.direction);
      this->set<float>(name + ".constant", light.constant);
      this->set<float>(name + ".linear", light.linear);
      this->set<float>(name + ".quadratic", light.quadratic);
      this->set<float>(name + ".innerCutOff",
                       glm::cos(glm::radians(light.innerCutOff)));
      this->set<float>(name + ".outerCutOff",
                       glm::cos(glm::radians(light.outerCutOff)));
      break;
    default:
      break;
    }
  }

  template <>
  void Shader::set<Material &, core::ResourceManager &>(
      const std::string &name, Material &material,
      core::ResourceManager &resourceManager) const {
    this->set<glm::vec3>(name + ".ambient", material.ambient.vector);
    this->set<glm::vec3>(name + ".diffuse", material.diffuse.vector);
    this->set<glm::vec3>(name + ".specular", material.specular.vector);
    this->set<float>(name + ".shininess", material.shininess);

    int ambientTexture = material.ambient.texture;
    this->set<bool>(name + ".ambientTexturePresent", (ambientTexture >= 0));
    if (ambientTexture >= 0) {
      resourceManager.getResourceByIndex<Texture>(ambientTexture).bind(0);
      this->set<int>(name + ".ambientTexture", 0);
    }

    int diffuseTexture = material.diffuse.texture;
    this->set<bool>(name + ".diffuseTexturePresent", (diffuseTexture >= 0));
    if (diffuseTexture >= 0) {
      resourceManager.getResourceByIndex<Texture>(diffuseTexture).bind(1);
      this->set<int>(name + ".diffuseTexture", 1);
    }

    int specularTexture = material.specular.texture;
    this->set<bool>(name + ".specularTexturePresent", (specularTexture >= 0));
    if (specularTexture >= 0) {
      resourceManager.getResourceByIndex<Texture>(specularTexture).bind(2);
      this->set<int>(name + ".specularTexture", 2);
    }
  }
}