#include "model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "base_object.hpp"
#include "materials.hpp"
#include "mesh.hpp"
#include "resource_manager.hpp"
#include "selectable.hpp"
#include "texture.hpp"
#include "transform.hpp"
#include "vertex.hpp"

namespace gl {
  Model::Model(std::vector<core::Vertex> &vertices,
               std::vector<unsigned int> &indices, MaterialGroup ambient,
               MaterialGroup diffuse, MaterialGroup specular,
               core::Registry &registryRef)
      : core::BaseObject(registryRef) {
    registryRef.addComponent<Mesh>(this->getEntityId(), vertices, indices);
    registryRef.addComponent<Material>(this->getEntityId(), ambient, diffuse,
                                       specular, 32.0f);
    registryRef.addComponent<core::Transform>(this->getEntityId());
    registryRef.addComponent<core::Selectable>(this->getEntityId());
  }
}

namespace gl {
  ModelLoader::ModelLoader(core::ResourceManager &resourceManager,
                           core::Registry        &registry)
      : resourceManager(resourceManager), registry(registry) {}

  void ModelLoader::processNode(aiNode *node, const aiScene *scene,
                                std::vector<Model> &models,
                                std::string        &directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      this->processMesh(mesh, scene, models, directory);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
      this->processNode(node->mChildren[i], scene, models, directory);
  }

  void ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene,
                                std::vector<Model> &models,
                                std::string        &directory) {
    std::vector<core::Vertex> vertices;
    this->processVertices(vertices, mesh);

    std::vector<unsigned int> indices;
    this->processIndices(indices, mesh);

    MaterialGroup ambient{.vector = glm::vec3{0.2f}};
    MaterialGroup diffuse{.vector = glm::vec3{0.4f}};
    MaterialGroup specular{.vector = glm::vec3{0.0f}};
    if (mesh->mMaterialIndex >= 0)
      this->processMaterials(ambient, diffuse, specular,
                             scene->mMaterials[mesh->mMaterialIndex],
                             directory);

    models.emplace_back(vertices, indices, ambient, diffuse, specular,
                        this->registry);
  }

  void ModelLoader::processVertices(std::vector<core::Vertex> &vertices,
                                    aiMesh                    *mesh) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      core::Vertex vertex{};
      vertex.position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z};
      vertex.normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z};
      if (mesh->mTextureCoords[0] != nullptr)
        vertex.textureCoordinates = glm::vec2{mesh->mTextureCoords[0][i].x,
                                              mesh->mTextureCoords[0][i].y};
      else
        vertex.textureCoordinates = glm::vec3{0.0f};
      vertices.push_back(vertex);
    }
  }

  void ModelLoader::processIndices(std::vector<unsigned int> &indices,
                                   aiMesh                    *mesh) {
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
  }

  void ModelLoader::processMaterials(MaterialGroup &ambient,
                                     MaterialGroup &diffuse,
                                     MaterialGroup &specular,
                                     aiMaterial    *material,
                                     std::string   &directory) {
    bool                     ambientPresent{false};
    bool                     diffusePresent{false};
    bool                     specularPresent{false};
    std::vector<std::string> paths{3};

    initializeTexture(aiTextureType_AMBIENT, material, directory, 0, paths,
                      ambientPresent);
    initializeTexture(aiTextureType_DIFFUSE, material, directory, 1, paths,
                      diffusePresent);
    initializeTexture(aiTextureType_SPECULAR, material, directory, 2, paths,
                      specularPresent);

    this->resourceManager.createResourcesBulk<Texture>(paths);
    this->emplaceTexture(ambientPresent, paths[0], ambient);
    this->emplaceTexture(diffusePresent, paths[1], diffuse);
    this->emplaceTexture(specularPresent, paths[2], specular);
  }

  void ModelLoader::initializeTexture(aiTextureType type, aiMaterial *material,
                                      std::string              &directory,
                                      unsigned int              index,
                                      std::vector<std::string> &paths,
                                      bool                     &present) {
    int count = material->GetTextureCount(type);
    if (count > 0) {
      aiString str;
      material->GetTexture(type, 0, &str);
      paths[index] = directory + '/' + str.C_Str();
      present = true;
    }
  }

  void ModelLoader::emplaceTexture(bool &present, std::string &path,
                                   MaterialGroup &group) {
    if (present)
      group.texture = this->resourceManager.getIndexOf<Texture>(path);
  }

  std::vector<Model> ModelLoader::loadModels(const std::string &path) {
    std::vector<Model> models{};
    Assimp::Importer   importer{};
    const aiScene     *scene = importer.ReadFile(
        path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
        !scene->mRootNode) {
      core::logger::error("Cannot load model {} due to {}", path,
                          importer.GetErrorString());
    } else {
      std::string directory = path.substr(0, path.find_last_of('/'));
      this->processNode(scene->mRootNode, scene, models, directory);
    }
    return models;
  }
}