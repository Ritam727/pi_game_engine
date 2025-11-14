#pragma once

#include "glad/glad.h"
#include "logger.hpp"

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

#define CHECK_GL_STATUS(shader, statusFunc, en, logFunc)                       \
  int  UNIQUE_NAME(success);                                                   \
  char UNIQUE_NAME(infoLog)[512];                                              \
  statusFunc(shader, en, &UNIQUE_NAME(success));                               \
  if (UNIQUE_NAME(success) == 0) {                                             \
    GL_CALL(logFunc(shader, 512, nullptr, UNIQUE_NAME(infoLog)));              \
    core::logger::error("Shader compilation failure for {}, details: {}",      \
                        #shader, UNIQUE_NAME(infoLog));                        \
  }

#include <signal.h>
#define dbg raise(SIGTRAP)

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    dbg;

#ifndef NDEBUG
#define GL_CALL(x)                                                             \
  gl::glClearError();                                                          \
  x;                                                                           \
  ASSERT(gl::glLogCall(__FILE__, #x, __LINE__))
#else
#define GL_CALL(x) x;
#endif

#ifndef NDEBUG
#define GL_CALL_WITH_ASSIGNMENT(x, var)                                        \
  gl::glClearError();                                                          \
  var = x;                                                                     \
  ASSERT(gl::glLogCall(__FILE__, #x, __LINE__))
#else
#define GL_CALL_WITH_ASSIGNMENT(x, var) var = x;
#endif

namespace gl {
  inline void glClearError() {
    while (glGetError() != GL_NO_ERROR)
      ;
  }

  inline bool glLogCall(const char *file, const char *function, int line) {
    bool value = true;
    while (GLenum error = glGetError()) {
      core::logger::error(
          "[OpenGL Error] ({}) in file {}, at function {}, on line {}", error,
          file, function, line);
      value = false;
    }
    return value;
  }
}