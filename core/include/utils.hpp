#pragma once

#include "logger.hpp"

#include <fstream>
#include <string>

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

#define CHECK_GL_STATUS(shader, statusFunc, en, logFunc)                       \
  int  UNIQUE_NAME(success);                                                   \
  char UNIQUE_NAME(infoLog)[512];                                              \
  statusFunc(shader, en, &UNIQUE_NAME(success));                               \
  if (UNIQUE_NAME(success) == 0) {                                             \
    logFunc(shader, 512, nullptr, UNIQUE_NAME(infoLog));                       \
    logger::error("Shader compilation failure for {}, details: {}", #shader,   \
                  UNIQUE_NAME(infoLog));                                       \
  }

namespace core {
  inline std::string readFromFile(char const *filePath) {
    std::ifstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
      file.open(filePath);
      std::stringstream fileStream;
      fileStream << file.rdbuf();
      file.close();
      return fileStream.str();
    } catch (std::ifstream::failure exp) {
      logger::error("Failed to read from file due to {}", exp.what());
      throw exp;
    }
  }
}