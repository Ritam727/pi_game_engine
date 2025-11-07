#include "texture.hpp"
#include "shader.hpp"

namespace core {
  Texture::Texture(const std::vector<std::string> filePaths)
      : textures(filePaths.size(), 0) {
    for (unsigned int i = 0; i < filePaths.size(); i++) {
      const std::string &filePath = filePaths[i];
      this->createBindAndConfigureTexture(i);
      Texture::loadImage(filePath);
    }
  }

  Texture::~Texture() {
    for (unsigned int i = 0; i < this->textures.size(); i++)
      glDeleteTextures(1, &this->textures[i]);
  }

  void Texture::createBindAndConfigureTexture(unsigned int index) {
    glGenTextures(1, &this->textures[index]);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, this->textures[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  void Texture::loadImage(const std::string &filePath) {
    stbi_set_flip_vertically_on_load(true);

    int            width, height, channels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                   (channels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      logger::error("Cannot load texture file {}", filePath);
    }

    stbi_image_free(data);
  }

  void Texture::bind() {
    for (unsigned int i = 0; i < this->textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, this->textures[i]);
    }
  }

  unsigned int Texture::getNumTextures() const {
    return this->textures.size();
  }
}