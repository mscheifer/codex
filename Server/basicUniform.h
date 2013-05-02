#include <GL/glew.h>
#include <iostream>
#include <array>
#include <map>
#include <cstring>
#include <memory>

namespace gx {
namespace uniform {
	class basic {
      const std::string varName;
  protected:
      std::map<GLuint,GLint> locations;
    public:
      basic(std::string name)
      : varName(std::move(name)) {}
      void addShader(GLuint shader, GLint loc) {
        locations.insert(std::make_pair(shader,loc));
      }
		  virtual void write(const GLfloat*) = 0;
      //virtual void write(const GLint*) = 0;
      virtual void update(GLuint) = 0;
	};
	class mat4f : public basic {
    std::array<GLfloat,4 * 4> storage;
    public:
      mat4f(std::string name)
      : basic(std::move(name)) {}
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void update(GLuint shader) {
        glUniformMatrix4fv(this->locations[shader],1,false,this->storage.data());
      }
	};
	class vec3 : public basic {
    std::array<GLfloat,3> storage;
    public:
      vec3(std::string name)
      : basic(std::move(name)) {}
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void update(GLuint shader) {
        glUniform3fv(this->locations[shader],1,this->storage.data());
      }
	};
  std::unique_ptr<basic> make_uniform(std::string name, GLenum type) {
    switch(type) {
      case GL_FLOAT_MAT4:
        return std::unique_ptr<basic>(new mat4f(name));
      default:
        std::cout << "make_uniform for: " << type << std::endl;
    }
    return std::unique_ptr<basic>(nullptr);
  }
} //end namespace uniform
} //end namespace gx