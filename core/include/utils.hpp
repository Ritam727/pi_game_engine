#pragma once

#include "logger.hpp"

#include <string>
#include <fstream>
#include <cxxabi.h>

#define ITERABLE_ENUM(name, ...) enum class name { __VA_ARGS__, END }
#define ENUM_VALUES(name) core::getEnumIterator<name>()
#define GET_NEXT_ENUM_VALUE(name, val) core::getNextEnumValue<name>(val)
#define GET_STATIC_TYPE(name) core::getStaticType<name>()
#define TYPE_STRING(t) #t

namespace core {
  template <typename T>
  concept HasNoneValue = requires { T::END; };

  template <typename T>
  concept IsIterableEnum = std::is_enum_v<T> && HasNoneValue<T>;

  template <typename C, typename B>
  concept IsSubClassOf = std::is_base_of_v<B, C>;

  template <typename T>
  concept IsEnum = std::is_enum_v<T>;
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

  template <IsIterableEnum T> inline T getNextEnumValue(T val) {
    unsigned int intVal = static_cast<unsigned int>(val);
    unsigned int endVal = static_cast<unsigned int>(T::END);
    intVal = (intVal + 1) % endVal;
    return static_cast<T>(intVal);
  }

  template <typename T> inline std::string getStaticType() {
    int         status;
    std::string concreteName = typeid(T).name();
    char       *demangledName =
        abi::__cxa_demangle(concreteName.c_str(), NULL, NULL, &status);
    if (status == 0) {
      concreteName = demangledName;
      std::free(demangledName);
    }
    return concreteName;
  }
}