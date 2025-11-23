#include "texture.hpp"
#include "gl_utils.hpp"

namespace gl {
  Texture::Texture(const std::string &path) : image(path) {
    this->path = path;
  }

  void Texture::createBindAndConfigureTexture() {
    GL_CALL(glGenTextures(1, &this->texture));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  }

  void Texture::bind(unsigned int activeTextureIndex) {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + activeTextureIndex));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture));
  }

  void Texture::sendImageToTexture(Image &image) {
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0,
                         (image.channels == 3) ? GL_RGB : GL_RGBA,
                         GL_UNSIGNED_BYTE, image.data));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
  }

  void Texture::loadResource() {
    this->image.load();
  }

  void Texture::initialize() {
    this->createBindAndConfigureTexture();
    this->sendImageToTexture(this->image);
    stbi_image_free(this->image.data);
  }

  void Texture::clear() {
    GL_CALL(glDeleteTextures(1, &this->texture));
  }
}