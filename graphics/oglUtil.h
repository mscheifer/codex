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

template<typename T>
constexpr GLenum typeVal() {
  return GL_INVALID_ENUM;
}

template <>
constexpr GLenum gx::typeVal<GLuint>();

template <>
constexpr GLenum gx::typeVal<GLfloat>();

} //end namespace gx
#endif
