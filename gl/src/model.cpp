#include "model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "base_object.hpp"
#include "materials.hpp"
#include "mesh.hpp"
#include "resource_manager.hpp"
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
  }

  void ModelLoader::processNode(aiNode *node, const aiScene *scene,
                                std::vector<Model>    &models,
                                std::string           &directory,
                                core::Registry        &registryRef,
                                core::ResourceManager &resourceManager) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      ModelLoader::processMesh(mesh, scene, models, directory, registryRef,
                               resourceManager);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
      ModelLoader::processNode(node->mChildren[i], scene, models, directory,
                               registryRef, resourceManager);
  }

  void ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene,
                                std::vector<Model>    &models,
                                std::string           &directory,
                                core::Registry        &registryRef,
                                core::ResourceManager &resourceManager) {
    std::vector<core::Vertex> vertices;
    std::vector<unsigned int> indices;

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

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    MaterialGroup ambient{.vector = glm::vec3{0.2f}};
    MaterialGroup diffuse{.vector = glm::vec3{0.4f}};
    MaterialGroup specular{.vector = glm::vec3{0.0f}};

    bool                     diffusePresent{false};
    bool                     specularPresent{false};
    std::vector<std::string> paths{2};

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial  *material = scene->mMaterials[mesh->mMaterialIndex];
      unsigned int diffuseTextureCount =
          material->GetTextureCount(aiTextureType_DIFFUSE);
      if (diffuseTextureCount >= 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        paths[0] = directory + '/' + str.C_Str();
        diffusePresent = true;
      }

      unsigned int specularTextureCount =
          material->GetTextureCount(aiTextureType_SPECULAR);
      if (specularTextureCount >= 0) {
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, 0, &str);
        paths[1] = directory + '/' + str.C_Str();
      }
    }

    resourceManager.createResources<Texture>(paths);

    if (diffusePresent) {
      ambient.texture.emplace(resourceManager.getResource<Texture>(paths[0]));
      diffuse.texture.emplace(resourceManager.getResource<Texture>(paths[0]));
    }
    if (specularPresent) {
      specular.texture.emplace(resourceManager.getResource<Texture>(paths[1]));
    }
    models.emplace_back(vertices, indices, ambient, diffuse, specular,
                        registryRef);
  }

  std::vector<Model>
      ModelLoader::loadModels(const std::string     &path,
                              core::Registry        &registryRef,
                              core::ResourceManager &resourceManager) {
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
      ModelLoader::processNode(scene->mRootNode, scene, models, directory,
                               registryRef, resourceManager);
    }
    return models;
  }
}