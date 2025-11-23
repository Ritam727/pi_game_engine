#pragma once

#include "glad/glad.h"

#include "logger.hpp"
#include "stb_image.hpp"

#include <string>
#include <unordered_map>

namespace gl {
  struct Image {
    int            width{0};
    int            height{0};
    int            channels{0};
    std::string    filePath;
    unsigned char *data{nullptr};

    Image() {}

    Image(const std::string filePath) : filePath(filePath) {
      stbi_set_flip_vertically_on_load(true);
      int width, height, channels;
      this->data = stbi_load(filePath.c_str(), &this->width, &this->height,
                             &this->channels, 0);
      if (data) {
        core::logger::info("Read image data from file {}", filePath);
      } else {
        core::logger::error("Cannot load texture file {}", filePath);
      }
    }
  };

  class ImageManager {
  public:
    static inline std::unordered_map<std::string, Image> loadedImages{};

    static Image createImage(std::string filePath);
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

  class TextureManager {
  public:
    static inline std::unordered_map<std::string, Texture> textureMap;

    static Texture createTexture(Image &image);
  };
}