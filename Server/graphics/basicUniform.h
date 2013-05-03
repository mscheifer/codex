#include <GL/glew.h>
#include <iostream>
#include <array>
#include <map>
#include <memory>
#include <cstring>
#include "shaderProgram.h"

namespace gx {
namespace uniform {
	class basic {
    protected:
      const std::string varName;
      std::map<GLuint,GLint> locations;
    public:
      basic(std::string name)
      : varName(std::move(name)) {}
      void addShader(shaderProgram* shader) {
        GLint loc = glGetUniformLocation(shader->progNum(), varName.c_str());
        locations.insert(std::make_pair(shader->progNum(),loc));
      }
		  virtual void write(const GLfloat*) = 0;
      virtual void write(const GLfloat)  = 0;
      //virtual void write(const GLint*) = 0;
      virtual void update(GLuint) const = 0;
      virtual std::string declaration() = 0;
	};
	class mat4f : public basic {
    std::array<GLfloat,4 * 4> storage;
    public:
      mat4f(std::string name)
      : basic(std::move(name)) {}
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void write(const GLfloat) {
        std::cout << "Error: only one value passed" << std::endl;
      }
      virtual void update(GLuint shader) const {
        glUniformMatrix4fv(this->locations.at(shader),1,false,this->storage.data());
        debugout << "glUniformMatrix4fv(" << this->locations.at(shader);
        debugout << ",1,false," << this->storage.data() << ");" << endl;
      }
      virtual std::string declaration() {
        return "mat4 " + this->varName + ";";
      }
	};
	class vec4f : public basic {
    std::array<GLfloat,4> storage;
    public:
      vec4f(std::string name)
      : basic(std::move(name)) {}
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void write(const GLfloat) {
        std::cout << "Error: only one value passed" << std::endl;
      }
      virtual void update(GLuint shader) const {
        glUniform4fv(this->locations.at(shader),1,this->storage.data());
      }
      virtual std::string declaration() {
        return "vec4 " + this->varName + ";";
      }
	};
  class vec1f : public basic {
    GLfloat storage;
    public:
      vec1f(std::string name)
      : basic(std::move(name)) {}
      virtual void write(const GLfloat* src) {
        storage = *src;
      }
      virtual void write(const GLfloat f) {
        storage = f;
      }
      virtual void update(GLuint shader) const {
        glUniform1f(this->locations.at(shader),this->storage);
      }
      virtual std::string declaration() {
        return "float " + this->varName + ";";
      }
	};
  inline std::unique_ptr<basic> make_uniform(std::string name, GLenum type) {
    switch(type) {
      case GL_FLOAT_MAT4:
        return std::unique_ptr<basic>(new mat4f(name));
      case GL_FLOAT_VEC4:
        return std::unique_ptr<basic>(new vec4f(name));
      case GL_FLOAT:
        return std::unique_ptr<basic>(new vec1f(name));
      default:
        std::cout << "make_uniform for: " << type << std::endl;
    }
    return std::unique_ptr<basic>(nullptr);
  }
} //end namespace uniform
} //end namespace gx