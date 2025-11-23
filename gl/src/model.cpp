#include "model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "base_object.hpp"
#include "materials.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "vertex.hpp"

namespace gl {
  Model::Model(std::vector<core::Vertex> &vertices,
               std::vector<unsigned int> &indices, MaterialAttribute ambient,
               MaterialAttribute diffuse, MaterialAttribute specular,
               core::Registry &registryRef)
      : core::BaseObject(registryRef) {
    registryRef.addComponent<Mesh>(this->getEntityId(), vertices, indices);
    registryRef.addComponent<Material>(this->getEntityId(), ambient, diffuse,
                                       specular, 32.0f);
    registryRef.addComponent<core::Transform>(this->getEntityId());
  }

  void ModelLoader::processNode(aiNode *node, const aiScene *scene,
                                std::vector<Model> &models,
                                std::string        &directory,
                                core::Registry     &registryRef) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      ModelLoader::processMesh(mesh, scene, models, directory, registryRef);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
      ModelLoader::processNode(node->mChildren[i], scene, models, directory,
                               registryRef);
  }

  void ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene,
                                std::vector<Model> &models,
                                std::string        &directory,
                                core::Registry     &registryRef) {
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

    MaterialAttribute ambient{glm::vec3{1.0f}};
    MaterialAttribute diffuse{glm::vec3{1.0f}};
    MaterialAttribute specular{glm::vec3{0.0f}};
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial  *material = scene->mMaterials[mesh->mMaterialIndex];
      unsigned int diffuseTextureCount =
          material->GetTextureCount(aiTextureType_DIFFUSE);
      if (diffuseTextureCount >= 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        ambient = MaterialAttribute{directory + '/' + str.C_Str()};
        diffuse = MaterialAttribute{directory + '/' + str.C_Str()};
      }

      unsigned int specularTextureCount =
          material->GetTextureCount(aiTextureType_SPECULAR);
      if (specularTextureCount >= 0) {
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, 0, &str);
        specular = MaterialAttribute{directory + '/' + str.C_Str()};
      }
    }
    models.emplace_back(vertices, indices, ambient, diffuse, specular,
                        registryRef);
  }

  std::vector<Model> ModelLoader::loadModels(const std::string &path,
                                             core::Registry    &registryRef) {
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
                               registryRef);
    }
    return models;
  }
}