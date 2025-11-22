#pragma once

#include "glad/glad.h"

#include "logger.hpp"
#include "stb_image.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace gl {
  struct Image {
    int            width;
    int            height;
    int            channels;
    unsigned char *data;
  };
}

namespace gl {
  class Texture {
  private:
    unsigned int texture;

    void createBindAndConfigureTexture();
    void generateTextures();
    void sendImageToTexture(Image &image);

    static void loadImage(const std::string &filePath, Image &images);

  public:
    Texture(const std::string filePath);

    void bind(unsigned int activeTextureIndex);
    void releaseTexture();
  };
}