#pragma once

#include "glad/glad.h"

#include "stb_image.hpp"
#include "logger.hpp"

#include <string>
#include <vector>

namespace core {
  class Texture {
  private:
    std::vector<unsigned int> textures;

    void createBindAndConfigureTexture(unsigned int index);

  public:
    Texture(const std::vector<std::string> filePaths);
    ~Texture();

    void        bind();
    static void loadImage(const std::string &filePath);

    unsigned int getNumTextures() const;
  };
}