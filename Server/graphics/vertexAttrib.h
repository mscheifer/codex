#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H
#include <vector>
#include "oglUtil.h"

namespace gx {

class vertexAttrib {
    std::string varName;
    GLint       vectorSize;
    GLenum      glType;
    GLsizei     byteOffset;
    GLuint      buffer;
  public:
    template<typename T>
    vertexAttrib(std::string n, GLint vs, GLsizei st, std::vector<T> d)
          : varName(n), vectorSize(vs), glType(typeVal(d[0])), byteOffset(st),
            buffer () {
      glGenBuffers(1, &buffer);
      debugout << "glGenBuffers(1, &buffer@" << &buffer << ");" << endl;
      this->bindBuffer();
      glBufferData(GL_ARRAY_BUFFER, d.size() * sizeof(T),
                                    d.data(), GL_STATIC_DRAW);
      debugout << "glBufferData(GL_ARRAY_BUFFER, " << d.size() * sizeof(T);
      debugout << ", d.data(), GL_STATIC_DRAW);" << endl;
    }
	//vc++ is dumb
    //vertexAttrib(const vertexAttrib&) = delete; //don't copy
    //vertexAttrib& operator=(const vertexAttrib&) = delete; //don't assign
    //vertexAttrib(vertexAttrib&&) = delete; //define later
    //vertexAttrib& operator=(vertexAttrib&&) = delete; //define later
    ~vertexAttrib();
    void bindBuffer()     const;
    std::string name()    const;
    GLint       vecSize() const;
    GLenum      type()    const;
    GLsizei     stride()  const;
};

} //end namespace gx
#endif