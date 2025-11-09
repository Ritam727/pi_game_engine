#pragma once

#include "logger.hpp"
#include "glad/glad.h"

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
    GL_CALL(logFunc(shader, 512, nullptr, UNIQUE_NAME(infoLog)));              \
    logger::error("Shader compilation failure for {}, details: {}", #shader,   \
                  UNIQUE_NAME(infoLog));                                       \
  }

#include <signal.h>
#define dbg raise(SIGTRAP)

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    dbg;

#ifndef NDEBUG
#define GL_CALL(x)                                                             \
  core::glClearError();                                                        \
  x;                                                                           \
  ASSERT(core::glLogCall(__FILE__, #x, __LINE__))
#else
#define GL_CALL(x) x;
#endif

#ifndef NDEBUG
#define GL_CALL_WITH_ASSIGNMENT(x, var)                                        \
  core::glClearError();                                                        \
  var = x;                                                                     \
  ASSERT(core::glLogCall(__FILE__, #x, __LINE__))
#else
#define GL_CALL_WITH_ASSIGNMENT(x, var) var = x;
#endif

namespace core {
  inline void glClearError() {
    while (glGetError() != GL_NO_ERROR)
      ;
  }

  inline bool glLogCall(const char *file, const char *function, int line) {
    bool value = true;
    while (GLenum error = glGetError()) {
      logger::error(
          "[OpenGL Error] ({0}) in file {1}, at function {2}, on line {3}",
          error, file, function, line);
      value = false;
    }
    return value;
  }

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