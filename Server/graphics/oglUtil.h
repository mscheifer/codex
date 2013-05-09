#ifndef OGLUTIL_H
#define OGLUTIL_H
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "windowsHacks.h"

namespace gx {

//don't set to true or it will break on Matt, Alvin and Bowen's machines
constexpr bool sharedUniformsOn = false;

constexpr bool debugOn = false;

const std::string shaderExtensions = 
  sharedUniformsOn ? "#extension GL_ARB_uniform_buffer_object : require\n" : "";

const std::string shaderHeader = 
  "#version 130\n" + shaderExtensions + "\n";

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

constexpr debugStream debugout = {};

const std::string endl = "\n";

inline constexpr GLenum typeVal(GLuint) {
  return GL_UNSIGNED_INT;
}

inline constexpr GLenum typeVal(GLfloat) {
  return GL_FLOAT;
}

GLint  typeComponents(GLenum type);
GLuint typeSize      (GLenum type);
GLenum baseType      (GLenum type);

} //end namespace gx
#endif
