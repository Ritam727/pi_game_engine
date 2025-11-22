#include "texture.hpp"
#include "gl_utils.hpp"

#include <mutex>
#include <thread>

namespace gl {
  Texture::Texture(const std::string filePath) {
    Image                    image;
    std::mutex               writeMutex;
    std::vector<std::thread> imageLoadJobs;
    Texture::loadImage(filePath, image);
    this->createBindAndConfigureTexture();
    this->sendImageToTexture(image);
    stbi_image_free(image.data);
  }

  void Texture::createBindAndConfigureTexture() {
    GL_CALL(glGenTextures(1, &this->texture));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  }

  void Texture::loadImage(const std::string &filePath, Image &image) {
    stbi_set_flip_vertically_on_load(true);
    int            width, height, channels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
      core::logger::debug("Read image data from file {}", filePath);
      image = Image(width, height, channels, data);
    } else {
      core::logger::error("Cannot load texture file {}", filePath);
    }
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

  void Texture::releaseTexture() {
    GL_CALL(glDeleteTextures(1, &this->texture));
  }
}