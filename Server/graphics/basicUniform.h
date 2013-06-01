#ifndef BASICUNIFROM_H
#define BASICUNIFROM_H
#include <GL/glew.h>
#include <array>
#include <vector>
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
      virtual void write(const GLuint) = 0;
      virtual void update(GLuint) const = 0;
      virtual std::string declaration() const = 0;
      virtual void addShader(const shaderProgram&) = 0;
  };
  template<GLenum Type>
  class _locations : public basic {
      unsigned int mSize;
    protected:
      std::map<GLuint,const loc<Type>> locations;
      const std::string varName;
      _locations(std::string,unsigned int);
      void updateLoc(GLuint shader, const GLfloat*) const;
      unsigned int size() const;
    public:
      void addShader(const shaderProgram&);
  };
  class mat4f : public _locations<GL_FLOAT_MAT4> {
      std::array<GLfloat,4 * 4> storage;
    public:
      mat4f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void write(const GLuint);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
  };
  class vec4f : public _locations<GL_FLOAT_VEC4> {
      std::array<GLfloat,4> storage;
    public:
      vec4f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void write(const GLuint);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
  };
  class vec4fArray : public _locations<GL_FLOAT_VEC4> {
      std::vector<GLfloat> storage;
    public:
      vec4fArray(std::string,unsigned int);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void write(const GLuint);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
  };
  class vec1f : public _locations<GL_FLOAT> {
      GLfloat storage;
      void updateLoc(GLuint shader, const GLfloat) const;
    public:
      vec1f(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat f);
      virtual void write(const GLuint);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
  };
  class vec1u : public _locations<GL_UNSIGNED_INT> {
      GLuint storage;
      void updateLoc(GLuint shader, const GLuint) const;
    public:
      vec1u(std::string);
      virtual void write(const GLfloat*);
      virtual void write(const GLfloat);
      virtual void write(const GLuint u);
      virtual void update(GLuint shader) const;
      virtual std::string declaration() const;
  };
  std::unique_ptr<basic> make_uniform(std::string, GLenum, unsigned int);
} //end namespace uniform
} //end namespace gx
#endif //BASICUNIFROM_H
