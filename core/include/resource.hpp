#pragma once

#include "utils.hpp"

#include <string>
#include <unordered_map>

namespace core {
  struct Resource {
    std::string path;

    virtual void initialize() = 0;
    virtual void clear() = 0;
    virtual void loadResource() = 0;
  };

  class IResourcePool {
  public:
    virtual ~IResourcePool() = default;
  };

  template <IsSubClassOf<Resource> T>
  class ResourcePool : public IResourcePool {
  private:
    std::unordered_map<std::string, unsigned int> resourceMap{};
    std::vector<T>                                resources;
    std::vector<unsigned int>                     poppedIndices;

  public:
    ~ResourcePool() {
      for (std::pair<const std::string, unsigned int> &p : this->resourceMap)
        this->resources[p.second].clear();
    }

    bool contains(const std::string &path) {
      return this->resourceMap.contains(path);
    }

    unsigned int getIndexOf(std::string &path) {
      return this->resourceMap.at(path);
    }

    T &getResourceByIndex(unsigned int index) {
      return this->resources[index];
    }

    T &get(const std::string &path) {
      const unsigned int idx = this->resourceMap.at(path);
      return this->resources[idx];
    }

    void createResource(const std::string &path) {
      if (!this->resourceMap.contains(path)) {
        if (this->poppedIndices.empty()) {
          this->resources.emplace_back(path);
          this->resourceMap.insert({path, this->resources.size() - 1});
        } else {
          unsigned int idx = this->poppedIndices.back();
          this->poppedIndices.pop_back();
          this->resources.emplace(this->resources.begin() + idx, path);
          this->resourceMap[path] = idx;
        }
      }
    }

    void deleteResource(const std::string &path) {
      if (this->resourceMap.contains(path)) {
        unsigned int currentIdx = this->resourceMap[path];
        this->resources[currentIdx].clear();
        this->resourceMap.erase(path);
        this->poppedIndices.emplace_back(currentIdx);
      }
    }
  };
}