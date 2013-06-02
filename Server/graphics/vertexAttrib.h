#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H
#include <vector>
#include <memory>
#include "buffer.h"
#include "oglUtil.h"

namespace gx {
class vertexAttrib {
    std::string varName;
    GLint       vectorSize;
    GLenum      glType;
    GLsizei     byteOffset;
    buffer      buff;
  protected:
    template<typename T>
    vertexAttrib(std::string n, GLint vs, GLsizei st,const std::vector<T>& d, GLenum hint)
          : varName(n), vectorSize(vs), glType(typeVal(d[0])), byteOffset(st),
            buff (GL_ARRAY_BUFFER, d, hint) {}
  public:
    typedef std::vector<std::shared_ptr<const vertexAttrib>> attribsList_t;
    template<typename T>
    vertexAttrib(std::string n, GLint vs, GLsizei st, std::vector<T> d)
          : varName(n), vectorSize(vs), glType(typeVal(d[0])), byteOffset(st),
            buff (GL_ARRAY_BUFFER, std::move(d), GL_STATIC_DRAW) {}
    vertexAttrib(const vertexAttrib&);// = delete; //don't copy
    vertexAttrib& operator=(const vertexAttrib&);// = delete; //don't assign
    vertexAttrib(vertexAttrib&&) noexcept;
    vertexAttrib& operator=(vertexAttrib&&);// = delete; //define later
    void bindBuffer()     const;
    std::string name()    const;
    GLint       vecSize() const;
    GLenum      type()    const;
    GLsizei     stride()  const;
};

class dynamicVertexAttrib : public vertexAttrib {
  public:
    template<typename T>
    dynamicVertexAttrib(std::string n, GLint vs, GLsizei st,const std::vector<T>& d)
         : vertexAttrib(std::move(n), vs, st, d, GL_STREAM_DRAW) {}
    template<typename T>
    void write(const std::vector<T>& d) {
      assert(this->type() == typeVal(d[0]));
      this->buff.write(d);
    }
};

} //end namespace gx
#endif
