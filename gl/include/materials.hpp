#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "texture.hpp"
#include <optional>
#include <thread>
#include <variant>

namespace gl {
  struct MaterialAttribute {
    enum class MaterialAttributeType { COLOR, TEXTURE } type;

    std::variant<glm::vec3, std::string> attribute;

    MaterialAttribute(glm::vec3 vec)
        : type(MaterialAttributeType::COLOR), attribute(vec) {}

    MaterialAttribute(std::string filePath)
        : type(MaterialAttributeType::TEXTURE), attribute(filePath) {}
  };
}

namespace gl {
  struct Material : public core::BaseComponent {
    glm::vec3              ambient{1.0f};
    std::optional<Texture> ambientTexture;
    glm::vec3              diffuse{1.0f};
    std::optional<Texture> diffuseTexture;
    glm::vec3              specular{1.0f};
    std::optional<Texture> specularTexture;

    float shininess;

    Material(MaterialAttribute ambient, MaterialAttribute diffuse,
             MaterialAttribute specular, float shininess)
        : shininess(shininess) {
      std::vector<std::thread> textureThreads;

      Image ambientImage{};
      Material::generateAttribute(ambient, this->ambient, ambientImage,
                                  textureThreads);

      Image diffuseImage{};
      Material::generateAttribute(diffuse, this->diffuse, diffuseImage,
                                  textureThreads);

      Image specularImage{};
      Material::generateAttribute(specular, this->specular, specularImage,
                                  textureThreads);

      for (std::thread &textureThread : textureThreads)
        textureThread.join();

      Material::placeImage(this->ambientTexture, ambientImage);
      Material::placeImage(this->diffuseTexture, diffuseImage);
      Material::placeImage(this->specularTexture, specularImage);
    }

    void clearComponent() override {
      if (this->ambientTexture.has_value())
        this->ambientTexture->releaseTexture();
      if (this->diffuseTexture.has_value())
        this->diffuseTexture->releaseTexture();
      if (this->specularTexture.has_value())
        this->specularTexture->releaseTexture();
    }

    static void generateAttribute(MaterialAttribute &attribute,
                                  glm::vec3 &vecAttribute, Image &image,
                                  std::vector<std::thread> &threads) {
      if (attribute.type == MaterialAttribute::MaterialAttributeType::TEXTURE) {
        threads.emplace_back([&]() {
          image = ImageManager::createImage(
              std::get<std::string>(attribute.attribute));
        });
      } else {
        vecAttribute = std::get<glm::vec3>(attribute.attribute);
      }
    }

    static void placeImage(std::optional<Texture> &texture, Image &image) {
      if (image.data != nullptr) {
        texture.emplace(TextureManager::createTexture(image));
      }
    }
  };
}