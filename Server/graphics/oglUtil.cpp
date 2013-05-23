#include "oglUtil.h"

gx::debugStream gx::debugout = {""};

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
    case GL_INT_VEC4:
      return 4;
    default:
      std::cout << "Add components for: " << typeToString(type) << std::endl;
      return 0;
  }
}

GLuint gx::typeSize(GLenum type) {
  switch(type) {
    case GL_FLOAT:
      return 4;
    case GL_INT:
      return 4;
    case GL_FLOAT_VEC2:
      return typeSize(GL_FLOAT) * 2;
    case GL_FLOAT_VEC3:
      return typeSize(GL_FLOAT) * 3;
    case GL_FLOAT_VEC4:
      return typeSize(GL_FLOAT) * 4;
    case GL_INT_VEC4:
      return typeSize(GL_INT) * 4;
    default:
      std::cout << "Add size for: " << typeToString(type) << std::endl;
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
    case GL_INT_VEC4:
      return GL_INT;
    default:
      std::cout << "Add base for: " << typeToString(type) << std::endl;
      return 0;
  }
}

std::string gx::typeToString(GLenum type) {
  switch(type) {
    case GL_FLOAT:
      return "GL_FLOAT";
    case GL_INT:
      return "GL_INT";
    case GL_UNSIGNED_INT:
      return "GL_UNSIGNED_INT";
    default:
      std::cout << "Add toString for: " << type << std::endl;
      return 0;
  }
}
