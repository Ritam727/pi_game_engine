#pragma once

#include "base_object.hpp"
#include "assimp/scene.h"
#include "materials.hpp"
#include "resource_manager.hpp"
#include "vertex.hpp"

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
    core::ResourceManager &resourceManager;
    core::Registry        &registry;

    void processNode(aiNode *node, const aiScene *scene,
                     std::vector<Model> &models, std::string &directory);
    void processMesh(aiMesh *mesh, const aiScene *scene,
                     std::vector<Model> &models, std::string &directory);
    void processVertices(std::vector<core::Vertex> &vertices, aiMesh *mesh);
    void processIndices(std::vector<unsigned int> &indices, aiMesh *mesh);
    void processMaterials(MaterialGroup &ambient, MaterialGroup &diffuse,
                          MaterialGroup &specular, aiMaterial *material,
                          std::string &directory);
    void initializeTexture(aiTextureType type, aiMaterial *material,
                           std::string &directory, unsigned int index,
                           std::vector<std::string> &paths, bool &present);
    void emplaceTexture(bool &present, std::string &path, MaterialGroup &group);

  public:
    ModelLoader(core::ResourceManager &resourceManager,
                core::Registry        &registry);

    std::vector<Model> loadModels(const std::string &path);
  };
}