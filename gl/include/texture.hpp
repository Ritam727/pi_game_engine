#pragma once

#include "resource.hpp"
#include "stb_image.hpp"

#include <string>

namespace gl {
  struct Image {
    int            width{0};
    int            height{0};
    int            channels{0};
    std::string    filePath;
    unsigned char *data{nullptr};

    Image(const std::string filePath) : filePath(filePath) {}

    void load() {
      stbi_set_flip_vertically_on_load(true);
      int width, height, channels;
      this->data = stbi_load(this->filePath.c_str(), &this->width,
                             &this->height, &this->channels, 0);
      if (data) {
        core::logger::info("Read image data from file {}", this->filePath);
      } else {
        core::logger::error("Cannot load texture file {}", this->filePath);
      }
    }
  };
}

namespace gl {
  class Texture : public core::Resource {
  private:
    Image        image;
    unsigned int texture;

    void createBindAndConfigureTexture();
    void generateTextures();
    void sendImageToTexture(Image &image);

  public:
    Texture(const std::string &path);

    void bind(unsigned int activeTextureIndex);

    void loadResource() override;
    void initialize() override;
    void clear() override;
  };
}