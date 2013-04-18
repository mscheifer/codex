#ifndef OGLUTIL_H
#define OGLUTIL_H
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "windowsHacks.h"

namespace gx {

const std::string shaderHeader = 
  "#version 130\n\
   #extension GL_ARB_uniform_buffer_object : require\n";

constexpr bool debugOn = true;

struct debugStream {
  template<typename T>
  const debugStream& operator<<(const T& a) const {
    if(debugOn) std::cout << a;
    GLenum err;
    while(gx::debugOn && (err = glGetError())) {
      std::cout << "OpenGL error: " << err << std::endl;
    }
    return *this;
  }
};

constexpr debugStream debugout;

const std::string endl = "\n";

inline constexpr GLenum typeVal(GLuint) {
  return GL_UNSIGNED_INT;
}

inline constexpr GLenum typeVal(GLfloat) {
  return GL_FLOAT;
}

} //end namespace gx
#endif
