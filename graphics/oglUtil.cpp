#include "oglUtil.h"

template <>
constexpr GLenum gx::typeVal<GLuint>() {
  return GL_UNSIGNED_INT;
}

template <>
constexpr GLenum gx::typeVal<GLfloat>() {
  return GL_FLOAT;
}