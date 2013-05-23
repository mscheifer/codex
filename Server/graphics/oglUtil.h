#ifndef OGLUTIL_H
#define OGLUTIL_H
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <sstream>
#include "ConfigManager.h"
#include "windowsHacks.h"
#include "util.h"

namespace gx {

//both these bools and the ConfigManager values need to be set to enable

//don't set to true or it will break on Matt, Alvin and Bowen's machines
constexpr bool sharedUniformsOn = false;

constexpr bool debugOn = false;

const std::string shaderExtensions = 
  sharedUniformsOn ? "#extension GL_ARB_uniform_buffer_object : require\n" : "";

const std::string shaderHeader = 
  "#version 130\n" + shaderExtensions + "\n";

struct debugStream {
  std::string logString;
  template<typename T>
  debugStream& operator<<(const T& a) {
    bool userDebugOn = StringToBool(ConfigManager::configMap["graphicsDebug"]);
    if(debugOn && userDebugOn) {
      std::cout << a;
      std::stringstream ss;
      ss << a;
      logString += ss.str();
      if(logString.back() == '\n') {
        logString = "";
        GLenum err;
        while((err = glGetError())) {
          std::cout << "OpenGL error: " << err << std::endl;
          std::stringstream sserror;
          sserror << err;
          ConfigManager::log(std::string("OpenGL error: ") + ss.str() + std::string("\n"));
        }
      }
    } 
    return *this;
  }
};

extern debugStream debugout;

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
