#include "texture.hpp"
#include "utils.hpp"

#include <mutex>
#include <thread>

namespace gl {
  Image::Image(int width, int height, int channels, unsigned char *data)
      : width(width), height(height), channels(channels), data(data) {}

  int Image::getWidth() {
    return this->width;
  }

  int Image::getHeight() {
    return this->height;
  }

  int Image::getChannels() {
    return this->channels;
  }

  unsigned char *Image::getData() {
    return this->data;
  }
}

namespace gl {
  Texture::Texture(const std::vector<std::string> filePaths)
      : textures(filePaths.size(), 0) {
    std::vector<Image>       images(filePaths.size());
    std::mutex               writeMutex;
    std::vector<std::thread> imageLoadJobs;
    for (unsigned int i = 0; i < filePaths.size(); i++) {
      const std::string &filePath = filePaths[i];
      imageLoadJobs.push_back(
          std::thread(Texture::loadImage, std::ref(filePaths[i]), i,
                      std::ref(images), std::ref(writeMutex)));
    }
    for (std::thread &job : imageLoadJobs)
      job.join();
    for (unsigned int i = 0; i < images.size(); i++) {
      this->createBindAndConfigureTexture(i);
      this->sendImageToTexture(images[i]);
      stbi_image_free(images[i].getData());
    }
  }

  Texture::~Texture() {
    for (unsigned int i = 0; i < this->textures.size(); i++) {
      GL_CALL(glDeleteTextures(1, &this->textures[i]));
    }
  }

  void Texture::createBindAndConfigureTexture(unsigned int index) {
    GL_CALL(glGenTextures(1, &this->textures[index]));
    GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, this->textures[index]));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  }

  void Texture::loadImage(const std::string &filePath, unsigned int index,
                          std::vector<Image> &images, std::mutex &mutex) {
    stbi_set_flip_vertically_on_load(true);
    int            width, height, channels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
      core::logger::info("Read image data from file {}", filePath);
      std::lock_guard<std::mutex> lock = std::lock_guard(mutex);
      images[index] = Image(width, height, channels, data);
    } else {
      core::logger::error("Cannot load texture file {}", filePath);
    }
  }

  void Texture::bind() {
    for (unsigned int i = 0; i < this->textures.size(); i++) {
      GL_CALL(glActiveTexture(GL_TEXTURE0 + i));
      GL_CALL(glBindTexture(GL_TEXTURE_2D, this->textures[i]));
    }
  }

  void Texture::sendImageToTexture(Image &image) {
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(),
                         image.getHeight(), 0,
                         (image.getChannels() == 3) ? GL_RGB : GL_RGBA,
                         GL_UNSIGNED_BYTE, image.getData()));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
  }

  unsigned int Texture::getNumTextures() const {
    return this->textures.size();
  }
}