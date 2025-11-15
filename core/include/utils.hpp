#pragma once

#include "logger.hpp"

#include <string>
#include <fstream>

#define iterableEnum(name, ...) enum class name { __VA_ARGS__, END }
#define enumValues(name) core::getEnumIterator<name>()

namespace core {
  template <typename T>
  concept HasNoneValue = requires { T::END; };

  template <typename T>
  concept IsIterableEnum = std::is_enum_v<T> && HasNoneValue<T>;
}

namespace core {
  inline std::string readFromFile(char const *filePath) {
    core::logger::info("Reading from file {}", filePath);
    std::ifstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
      file.open(filePath);
      std::stringstream fileStream;
      fileStream << file.rdbuf();
      file.close();
      return fileStream.str();
    } catch (std::ifstream::failure exp) {
      core::logger::error("Failed to read from file {} due to {}", filePath,
                          exp.what());
      throw exp;
    }
  }

  template <IsIterableEnum T> inline std::vector<T> getEnumIterator() {
    std::vector<T> vec(static_cast<unsigned int>(T::END), T::END);
    for (unsigned int i = 0; i < static_cast<unsigned int>(T::END); i++) {
      vec[i] = static_cast<T>(i);
    }
    return vec;
  }
}