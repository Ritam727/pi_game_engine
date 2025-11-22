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
    std::unordered_map<std::string, unsigned int> indexes;
    std::vector<unsigned int>                     textures;

    void createBindAndConfigureTexture(unsigned index);
    void generateTextures();

  public:
    Texture(const std::vector<std::string> filePaths);

    void bind();
    void sendImageToTexture(Image &image);

    unsigned int getNumTextures() const;

    static void loadImage(const std::string &filePath, unsigned int index,
                          std::vector<Image> &images, std::mutex &mutex);
    void        releaseTextures();
  };
}