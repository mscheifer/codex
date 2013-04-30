#include "oglUtil.h"

GLint gx::typeComponents(GLenum type) {
  switch(type) {
    case GL_FLOAT:
      return 1;
    case GL_FLOAT_VEC2:
      return 2;
    case GL_FLOAT_VEC3:
      return 3;
    case GL_FLOAT_VEC4:
      return 4;
    default:
      std::cout << "Add components for: " << type << std::endl;
      return 0;
  }
}

GLenum gx::baseType(GLenum type) {
  switch(type) {
    case GL_FLOAT:
      return GL_FLOAT;
    case GL_FLOAT_VEC2:
      return GL_FLOAT;
    case GL_FLOAT_VEC3:
      return GL_FLOAT;
    case GL_FLOAT_VEC4:
      return GL_FLOAT;
    default:
      std::cout << "Add base for: " << type << std::endl;
      return 0;
  }
}
