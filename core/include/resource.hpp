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
    std::unordered_map<std::string, T> resourceMap{};

  public:
    ~ResourcePool() {
      for (std::pair<const std::string, T> &p : this->resourceMap)
        p.second.clear();
    }

    bool contains(const std::string &path) {
      return this->resourceMap.contains(path);
    }

    T &get(const std::string &path) {
      return this->resourceMap.at(path);
    }

    void createResource(const std::string &path) {
      if (!this->resourceMap.contains(path))
        this->resourceMap.insert({path, T{path}});
    }

    void deleteResource(const std::string &path) {
      if (this->resourceMap.contains(path)) {
        this->resourceMap[path].clear();
        this->resourceMap.erase(path);
      }
    }
  };
}