#ifndef BASICUNIFROM_H
#define BASICUNIFROM_H
#include <GL/glew.h>
#include <array>
#include <map>
#include <memory>
#include "uniformLocation.h"

namespace gx {
class shaderProgram;

namespace uniform {
	class basic {
    public:
		  virtual void write(const GLfloat*) = 0;
      virtual void write(const GLfloat)  = 0;
      //virtual void write(const GLint*) = 0;
      virtual void update(GLuint) const = 0;
      virtual std::string declaration() const = 0;
      virtual void addShader(const shaderProgram&) = 0;
	};
  template<GLenum Type>
  class _locations : public basic {
      std::map<GLuint,loc<Type>> locations;
    protected:
      const std::string varName;
      _locations(std::string);
      void updateLoc(GLuint shader, const GLfloat*) const;
      void updateLoc(GLuint shader, const GLfloat ) const;
    public:
      void addShader(const shaderProgram&);
  };
	class mat4f : public _locations<GL_FLOAT_MAT4> {
      std::array<GLfloat,4 * 4> storage;
    public:
      mat4f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
	};
	class vec4f : public _locations<GL_FLOAT_VEC4> {
      std::array<GLfloat,4> storage;
    public:
      vec4f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
	};
  class vec1f : public _locations<GL_FLOAT> {
      GLfloat storage;
    public:
      vec1f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat f);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
	};
  std::unique_ptr<basic> make_uniform(std::string, GLenum);
} //end namespace uniform
} //end namespace gx
#endif //BASICUNIFROM_H
