#ifndef OGLUTIL_H
#define OGLUTIL_H
#include <GL/glew.h>
#include <iostream>

namespace gx {

const std::string shaderHeader = 
  "#version 130\n#extension GL_ARB_uniform_buffer_object : require\n";

constexpr bool debugOn = false;

struct debugStream {
  template<typename T>
  const debugStream& operator<<(const T& a) const {
    if(debugOn) std::cout << a;
    return *this;
  }
};

constexpr debugStream debugout;

const std::string endl = "\n";

template<typename T>
constexpr GLenum typeVal() {
  return GL_INVALID_ENUM;
}

template <>
constexpr GLenum typeVal<GLuint>() {
  return GL_UNSIGNED_INT;
}

template <>
constexpr GLenum typeVal<GLfloat>() {
  return GL_FLOAT;
}

} //end namespace gx
#endif
