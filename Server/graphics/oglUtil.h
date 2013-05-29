#ifndef OGLUTIL_H
#define OGLUTIL_H
#include <GL/glew.h>
#include <assert.h>
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

#ifdef GRAPHICSDEBUG
constexpr bool debugOn = true;
#else
constexpr bool debugOn = false;
#endif

const std::string shaderExtensions = 
  sharedUniformsOn ? "#extension GL_ARB_uniform_buffer_object : require\n" : "";

const std::string shaderHeader = 
  "#version 130\n" + shaderExtensions + "\n";

class debugStream {
    std::stringstream logString;
  public:
    template<typename T>
    debugStream& operator<<(const T& a) {
      bool userDebugOn= StringToBool(ConfigManager::configMap["graphicsDebug"]);
      if(debugOn && userDebugOn) {
        logString << a;
        if(logString.str().back() == '\n') {
          std::string toPrint;
          getline(logString, toPrint);
          std::cout << toPrint << std::endl;
          GLenum err;
          while((err = glGetError())) {
            std::stringstream sserror;
            sserror << "OpenGL error: " << err << std::endl;
            std::cout << sserror.str();
            ConfigManager::log(sserror.str());
            assert(false);
          }
        }
      } 
      return *this;
    }
};

extern debugStream debugout;

const std::string endl = "\n";

inline constexpr GLenum typeVal(GLint) {
  return GL_INT;
}

inline constexpr GLenum typeVal(GLuint) {
  return GL_UNSIGNED_INT;
}

inline constexpr GLenum typeVal(GLfloat) {
  return GL_FLOAT;
}

GLint       typeComponents(GLenum type);
GLuint      typeSize      (GLenum type);
GLenum      baseType      (GLenum type);
std::string typeToString  (GLenum type);

} //end namespace gx
#endif
