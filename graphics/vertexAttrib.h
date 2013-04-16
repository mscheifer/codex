#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H
#include <vector>
#include "oglUtil.h"

namespace gx {

class vertexAttrib {
    GLuint  buffer;
    GLuint  shaderLocation;
    GLint   vectorSize;
    GLenum  glType;
    GLsizei byteOffset;
  public:
    template<typename T>
    vertexAttrib(GLuint loc, GLint vs, GLsizei st, std::vector<T> d)
    : shaderLocation(loc), vectorSize(vs), glType(typeVal<T>()), byteOffset(st){
      debugout << "glGenBuffers(1, &buffer);" << endl;
      glGenBuffers(1, &buffer);
      this->bindBuffer();
      debugout << "glBufferData(GL_ARRAY_BUFFER, " << d.size() * sizeof(T);
      debugout << ", d.data(), GL_STATIC_DRAW);" << endl;
      glBufferData(GL_ARRAY_BUFFER, d.size() * sizeof(T),
                                    d.data(), GL_STATIC_DRAW);
    }
    void bindBuffer() const;
    GLuint  loc()     const;
    GLint   vecSize() const;
    GLenum  type()    const;
    GLsizei stride()  const;
};

} //end namespace gx
#endif
