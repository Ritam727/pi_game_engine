#pragma once

#include "glad/glad.h"

#include "logger.hpp"
#include "stb_image.hpp"

#include <string>
#include <vector>

namespace gl {
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