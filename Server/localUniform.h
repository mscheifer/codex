#include <GL/glew.h>
#include <array>
#include <cstring>

namespace gx {
namespace uniform {
	class local {
    public:
		  virtual void write(const GLfloat*) = 0;
      //virtual void write(const GLint*) = 0;
      virtual void update(GLint) = 0;
	};
	class mat4f : public local {
    std::array<GLfloat,4 * 4> storage;
    public:
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void update(GLint loc) {
        glUniformMatrix4fv(loc,1,false,this->storage.data());
      }
	};
	class vec3 : public local {
    std::array<GLfloat,3> storage;
    public:
      virtual void write(const GLfloat* src) {
        std::memcpy(storage.data(),src,sizeof(storage));
      }
      virtual void update(GLint loc) {
        glUniform3fv(loc,1,this->storage.data());
      }
	};
} //end namespace uniform
} //end namespace gx