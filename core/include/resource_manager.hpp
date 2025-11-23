#pragma once

#include "resource.hpp"
#include "utils.hpp"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

namespace core {
  class ResourceManager {
  private:
    std::unordered_set<std::string> registeredTypes;
    std::unordered_map<std::type_index, std::unique_ptr<IResourcePool>>
        resources;

  public:
    ResourceManager() {}

    template <IsSubClassOf<Resource> T>
    void createResources(std::vector<std::string> &paths) {
      std::type_index           poolIndex = std::type_index(typeid(T));
      std::vector<std::thread>  threadJobs;
      std::vector<unsigned int> indices;
      unsigned int              index = 0;

      if (!this->resources.contains(poolIndex))
        this->resources.insert(
            {poolIndex, std::make_unique<ResourcePool<T>>()});

      ResourcePool<T> *pool =
          static_cast<ResourcePool<T> *>(this->resources[poolIndex].get());

      for (std::string &path : paths) {
        if (!pool->contains(path)) {
          pool->createResource(path);
          indices.push_back(index);
          threadJobs.emplace_back([&] { pool->get(path).loadResource(); });
        }
        index++;
      }
      for (std::thread &job : threadJobs)
        job.join();
      for (unsigned int i : indices) {
        pool->get(paths[i]).initialize();
      }
    }

    template <IsSubClassOf<Resource> T>
    T &getResource(const std::string &path) {
      std::type_index poolIndex = std::type_index(typeid(T));
      return static_cast<ResourcePool<T> *>(this->resources[poolIndex].get())
          ->get(path);
    }

    template <IsSubClassOf<Resource> T>
    void deleteResource(const std::string &path) {
      std::type_index poolIndex = std::type_index(typeid(T));
      core::logger::info("Removing resource path {}", path);
      if (!this->resources.contains(poolIndex))
        return;
      static_cast<ResourcePool<T> *>(this->resources[poolIndex].get())
          ->deleteResource(path);
    }
  };
}