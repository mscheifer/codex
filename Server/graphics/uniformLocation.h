#ifndef UNIFORMLOCATION_H
#define UNIFORMLOCATION_H
#include <GL/glew.h>
#include <string>

namespace gx {
class shaderProgram;
namespace uniform {
template<GLenum Type>
class loc {
};
template <>
class loc<GL_FLOAT_MAT4> {
    GLint   location;
    GLsizei     size;
  public:
    loc(const shaderProgram&,std::string,GLsizei);
    void write(const GLfloat*) const;
    void write(const GLfloat*,GLsizei) const;
    void write(const GLfloat) const;
};
template <>
class loc<GL_FLOAT_VEC4> {
    GLint  location;
    GLsizei     size;
  public:
    loc(const shaderProgram&,std::string,GLsizei);
    void write(const GLfloat*) const;
    void write(const GLfloat*,GLsizei) const;
    void write(const GLfloat) const;
};
template <>
class loc<GL_FLOAT> {
    GLint  location;
    GLsizei     size;
  public:
    loc(const shaderProgram&,std::string,GLsizei);
    void write(const GLfloat*) const;
    void write(const GLfloat*,GLsizei) const;
    void write(const GLfloat) const;
};

typedef loc<GL_FLOAT_MAT4> mat4floc;
} //end namespace uniform
} //end namespace gx
#endif //UNIFORMLOCATION_H
