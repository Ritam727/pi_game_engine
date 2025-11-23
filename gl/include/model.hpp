#pragma once

#include "base_object.hpp"
#include "assimp/scene.h"
#include "materials.hpp"
#include "resource_manager.hpp"
#include "vertex.hpp"

#include <optional>
#include <vector>

namespace gl {
  class Model : core::BaseObject {
  public:
    Model(std::vector<core::Vertex> &vertices,
          std::vector<unsigned int> &indices, MaterialGroup ambient,
          MaterialGroup diffuse, MaterialGroup specular,
          core::Registry &registryRef);
  };

  class ModelLoader {
  private:
    static void processNode(aiNode *node, const aiScene *scene,
                            std::vector<Model> &models, std::string &directory,
                            core::Registry        &registryRef,
                            core::ResourceManager &resourceManager);
    static void processMesh(aiMesh *mesh, const aiScene *scene,
                            std::vector<Model> &models, std::string &directory,
                            core::Registry        &registryRef,
                            core::ResourceManager &resourceManager);

  public:
    static std::vector<Model>
        loadModels(const std::string &path, core::Registry &registryRef,
                   core::ResourceManager &resourceManager);
  };
}