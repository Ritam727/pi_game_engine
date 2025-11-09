#pragma once

#include "glad/glad.h"

#include "logger.hpp"
#include "stb_image.hpp"

#include <string>
#include <vector>

namespace gl {
  class Image {
  private:
    int            width;
    int            height;
    int            channels;
    unsigned char *data;

  public:
    Image() = default;
    Image(int width, int height, int channels, unsigned char *);

    int getWidth();
    int getHeight();
    int getChannels();

    unsigned char *getData();
  };
}

namespace gl {
  class Texture {
  private:
    std::vector<unsigned int> textures;

    void createBindAndConfigureTexture(unsigned int index);

  public:
    Texture(const std::vector<std::string> filePaths);
    ~Texture();

    void bind();
    void sendImageToTexture(Image &image);

    unsigned int getNumTextures() const;

    static void loadImage(const std::string &filePath, unsigned int index,
                          std::vector<Image> &images, std::mutex &mutex);
  };
}