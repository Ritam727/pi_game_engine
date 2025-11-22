#pragma once

#include "glad/glad.h"

#include "logger.hpp"
#include "stb_image.hpp"

#include <string>

namespace gl {
  struct Image {
    int            width{0};
    int            height{0};
    int            channels{0};
    unsigned char *data{nullptr};

    Image() {}

    Image(const std::string filePath) {
      stbi_set_flip_vertically_on_load(true);
      int width, height, channels;
      this->data = stbi_load(filePath.c_str(), &this->width, &this->height,
                             &this->channels, 0);
      if (data) {
        core::logger::debug("Read image data from file {}", filePath);
      } else {
        core::logger::error("Cannot load texture file {}", filePath);
      }
    }
  };
}

namespace gl {
  class Texture {
  private:
    unsigned int texture;

    void createBindAndConfigureTexture();
    void generateTextures();
    void sendImageToTexture(Image &image);

  public:
    Texture(Image &image);

    void bind(unsigned int activeTextureIndex);
    void releaseTexture();
  };
}